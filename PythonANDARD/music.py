import numpy as np
import pyaudio
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import serial
import threading

# Audio parameters
FORMAT = pyaudio.paFloat32
CHANNELS = 1
RATE = 44100
CHUNK = 1024 * 4  # Window size for FFT
OVERLAP = 512  # Overlap between windows
BANDS = [
    (30, 200),
    (60, 240),
    (250, 480),
    (500, 770),
    (800, 1060),
    (1100, 1550),
    (1600, 2420),
    (2500, 3900),
    (4000, 8000),
]

BEAT_THRESHOLD = 0.76
BEAT_MIN_INTERVAL = 0.8  # Minimum seconds between beats to prevent double counting
last_beat_time = time.time()




NORMALIZATION_PERIOD = 5  # Seconds for normalization history
MOVING_AVERAGE_LENGTH = int(NORMALIZATION_PERIOD * RATE / CHUNK)  # Number of frames

# Initialize PyAudio
p = pyaudio.PyAudio()
SERIAL_PORT = 'COM3'  # Adjust as per your system
BAUD_RATE = 115200

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
except Exception as e:
    print(f"Failed to connect to Arduino on {SERIAL_PORT}: {e}")
    exit(1)


# Global state for normalization and beat detection
band_powers_history = np.zeros((len(BANDS), MOVING_AVERAGE_LENGTH))
audio_buffer = np.zeros(CHUNK)

def frequency_to_bin(frequency):
    CALIBRATION_FACTOR = 1.6  # Adjust frequency based on calibration
    corrected_frequency = float(frequency) * CALIBRATION_FACTOR
    return int((corrected_frequency * CHUNK) / RATE)

def set_bar_heights(bars, heights):
    """
    Set the heights of the bars in a matplotlib bar chart.
    
    Parameters:
        bars (matplotlib.container.BarContainer): The bar container object.
        heights (list or np.ndarray): An array of heights to set for each bar.
    """
    for bar, height in zip(bars, heights):
        bar.set_height(height)

def send_volume_and_beat_to_arduino(serial_connection, volumes, beat_detected):
    try:
        # Convert the volume levels to bytes
        if beat_detected:
            serial_connection.write(b'B')  # Sending 'B' as the beat signal
        volume_bytes = bytes(int(v) for v in volumes)  # Ensure volumes are scaled from 0 to 10
        serial_connection.write(volume_bytes)
        
        # Send a beat signal if detected

    except Exception as e:
        print(f"Failed to send data to Arduino: {e}")



def is_beat(band_power_db, band_history, band_idx):
    global last_beat_time
    current_time = time.time()
    # Only consider a beat in the first band (60-240 Hz)
    if band_idx != 0:  # Skip beat detection for other bands
        return False
    if current_time - last_beat_time > BEAT_MIN_INTERVAL:
        max_power_db = max(band_history)
        threshold = max_power_db * BEAT_THRESHOLD
        if band_power_db > threshold:
            last_beat_time = current_time
            return True
    return False

# Visualization setup
fig, ax = plt.subplots()
band_centers = [(start + end) / 2 for start, end in BANDS]
x = np.arange(len(BANDS))
y = np.zeros(len(BANDS))
bars = plt.bar(x, y, color='blue')
ax.set_xticks(x)
ax.set_xticklabels([f"{center:.0f} Hz" for center in band_centers], rotation=45, ha='right')
ax.set_ylim(0, 10)  # dB range
ax.set_xlim(-0.5, len(BANDS) - 0.5)
ax.set_xlabel("Frequency Band")
ax.set_ylabel("Amplitude (0-10)")

def audio_callback(in_data, frame_count, time_info, status):
    global band_powers_history, audio_buffer
    data = np.frombuffer(in_data, dtype=np.float32)
    combined_data = np.concatenate((audio_buffer, data))
    audio_buffer = data[-OVERLAP:]
    combined_data -= np.mean(combined_data)

    # Apply Blackman window
    window = np.blackman(len(combined_data))
    fft_result = np.fft.fft(combined_data * window)[:len(combined_data) // 2]
    fft_magnitude = np.abs(fft_result)

    # Calculate power in dB for the selected band and update history
    band_powers_db = []
    for (start_freq, end_freq) in BANDS:
        start_bin = frequency_to_bin(start_freq)
        end_bin = frequency_to_bin(end_freq) - 1
        band_power = np.sum(fft_magnitude[start_bin:end_bin] ** 2)
        power_db = 10 * np.log10(band_power) if band_power > 0.1 else 0
        band_powers_db.append(power_db)

    band_powers_history = np.roll(band_powers_history, -1, axis=1)
    band_powers_history[:, -1] = band_powers_db

    beat_detected = False
    for band_idx, band_power_db in enumerate(band_powers_db):
        band_history = band_powers_history[band_idx, :]
        if is_beat(band_power_db, band_history, band_idx):
            print(f"Beat detected at band {BANDS[band_idx][0]}-{BANDS[band_idx][1]} Hz")
            beat_detected = True

    normalized_powers = [db / max(band_powers_history[0, :]) if max(band_powers_history[0, :]) else db for db in band_powers_db]
    scaled_normalized_powers = np.clip(np.array(normalized_powers) * 10, 0, 10)
    set_bar_heights(bars, scaled_normalized_powers)

    # Send data to Arduino
    send_volume_and_beat_to_arduino(ser, scaled_normalized_powers, beat_detected)

    return (in_data, pyaudio.paContinue)


def open_stream(callback):
    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK,
                    stream_callback=callback,
                    input_device_index=1)
    return stream

stream = open_stream(audio_callback)

def update_fig(frame):
    return [bar for bar in bars]

ani = animation.FuncAnimation(fig, update_fig, blit=True, interval=50, save_count=100)

try:
    plt.show()
finally:
    stream.stop_stream()
    stream.close()
    p.terminate()
