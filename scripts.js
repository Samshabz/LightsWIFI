
//contents
// default listeners
// rgb slider
// hue slider
// toggle slider func
// button active highlight
// transmit bits func



//default listeners 
document.getElementById('patt3').addEventListener('click', function() {
    toggleSliders('');
    sendColorBits('0002');
});

document.getElementById('patt4').addEventListener('click', function() {
    toggleSliders('');
    sendColorBits('0003');
});

document.getElementById('patt5').addEventListener('click', function() {
    toggleSliders('');
    sendColorBits('0004');
    
});

//turn on rgB sliders and update color
document.getElementById('solidcolor').addEventListener('click', function() {
    toggleSliders('colorPicker');
    updateColor();
});


// RGB slider active 

['red', 'green', 'blue'].forEach(color => {
    document.getElementById(color + 'Slider').addEventListener('input', function() {
        document.getElementById(color + 'Value').value = this.value;
        updateColor();
    });

    document.getElementById(color + 'Value').addEventListener('input', function() {
        document.getElementById(color + 'Slider').value = this.value;
        updateColor();
    });
});

function updateColor() {
    let red = document.getElementById('redValue').value;
    let green = document.getElementById('greenValue').value;
    let blue = document.getElementById('blueValue').value;
    sendColorBits('0000' + red.toString(16).padStart(2, '0') + green.toString(16).padStart(2, '0') + blue.toString(16).padStart(2, '0'));
}




//hue updater
document.getElementById('HUE').addEventListener('click', function() {
    toggleSliders('huePicker');
    updateHue();
    
   
});

['brightness', 'warmth'].forEach(property => {
    document.getElementById(property + 'Slider').addEventListener('input', function() {
        document.getElementById(property + 'Value').value = this.value;
        updateHue();
    });

    document.getElementById(property + 'Value').addEventListener('input', function() {
        document.getElementById(property + 'Slider').value = this.value;
        updateHue();
    });
});

function updateHue() {
    let brightness = document.getElementById('brightnessValue').value;
    let warmth = document.getElementById('warmthValue').value;
    sendColorBits('0001' + brightness.padStart(2, '0') + warmth.padStart(2, '0'));
}


function toggleSliders(activeSliderId) {
    // Hide all sliders with the class 'sliders'
    document.querySelectorAll('.sliders').forEach(slider => {
        slider.style.display = 'none';
    });

    // Show only the active slider, if specified
    if (activeSliderId && document.getElementById(activeSliderId)) {
        document.getElementById(activeSliderId).style.display = 'block';
    }
}



// make button highlight active
let buttons = document.querySelectorAll('.button');
buttons.forEach(btn => {
    btn.addEventListener('click', function() {
        buttons.forEach(b => b.classList.remove('active')); // Remove active from all buttons
        this.classList.add('active'); // Add active to clicked button
    });
});


//update and send packet



function sendColorBits(bits) {
    document.getElementById('debugValue').innerText = 'Hex: ' + bits;
    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log('Sent bits: ' + bits);
            console.log(this.responseText);
         
        }
    };
    xhttp.open("GET", "http://" + window.location.hostname + "/command?bits=" + bits, true);
    xhttp.send();
  
}

