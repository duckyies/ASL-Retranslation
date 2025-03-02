const darkModeToggle = document.getElementById('darkModeToggle');

const savedDarkMode = localStorage.getItem('darkMode');
if (savedDarkMode === 'true') {
    document.body.classList.add('dark-mode');
    darkModeToggle.checked = true;
}

darkModeToggle.addEventListener('change', () => {
    document.body.classList.toggle('dark-mode');

    localStorage.setItem('darkMode', darkModeToggle.checked);
});

document.getElementById('translatorForm').addEventListener('submit', async function(e) {
    e.preventDefault();
    const inputText = document.getElementById('inputText').value;
    
    let preprocessedText = await fetch("http://127.0.0.1:5000/process", {method:"POST", headers: new Headers({'content-type': 'application/json'}), body: JSON.stringify({text : inputText})})
    preprocessedText = await preprocessedText.json()

    document.getElementById('preprocessedOutput').textContent = preprocessedText[1];
    
    displayTokens(preprocessedText[0]);
    
    animateFingers(preprocessedText[0]);
});

function displayTokens(tokens) {
    const tokensDisplay = document.getElementById('tokensOutput');
    tokensDisplay.innerHTML = tokens.map(token => 
        token === '' ? '&nbsp;<span class="badge bg-secondary">space</span>&nbsp;' : 
        `<span class="badge bg-primary">${token}</span>`
    ).join(' ');
}

function animateFingers(tokens) {
    let currentIndex = 0;
    const letterDisplay = document.getElementById('currentLetter');
    
    if (window.animationInterval) {
        clearInterval(window.animationInterval);
    }
    
    function updateFingers(letter) {
        letterDisplay.textContent = letter === '' ? 'Space' : letter.toUpperCase();
        
        const fingerElements = {
            thumb: document.getElementById('thumb'),
            index: document.getElementById('index'),
            middle: document.getElementById('middle'),
            ring: document.getElementById('ring'),
            pinky: document.getElementById('pinky')
        };
        
        Object.values(fingerElements).forEach(el => {
            el.style.transform = 'rotate(0deg)';
        });
        
        if (letter === '') {
            return;
        }

        switch (letter.toLowerCase()) {
            case 'a':
                fingerElements.thumb.style.transform = 'rotate(45deg)';
                fingerElements.index.style.transform = 'rotate(90deg)';
                fingerElements.middle.style.transform = 'rotate(90deg)';
                fingerElements.ring.style.transform = 'rotate(90deg)';
                fingerElements.pinky.style.transform = 'rotate(90deg)';
                break;
            case 'b':
                fingerElements.thumb.style.transform = 'rotate(90deg)';
                break;
            case 'c':
                fingerElements.thumb.style.transform = 'rotate(45deg)';
                fingerElements.index.style.transform = 'rotate(45deg)';
                fingerElements.middle.style.transform = 'rotate(45deg)';
                fingerElements.ring.style.transform = 'rotate(45deg)';
                fingerElements.pinky.style.transform = 'rotate(45deg)';
                break;
            case 'i':
                fingerElements.thumb.style.transform = 'rotate(45deg)';
                fingerElements.index.style.transform = 'rotate(90deg)';
                fingerElements.middle.style.transform = 'rotate(90deg)';
                fingerElements.ring.style.transform = 'rotate(90deg)';
                break;
            case 'l':
                fingerElements.thumb.style.transform = 'rotate(90deg)';
                fingerElements.index.style.transform = 'rotate(0deg)';
                fingerElements.middle.style.transform = 'rotate(90deg)';
                fingerElements.ring.style.transform = 'rotate(90deg)';
                fingerElements.pinky.style.transform = 'rotate(90deg)';
                break;
            case 'o':
                fingerElements.thumb.style.transform = 'rotate(45deg)';
                fingerElements.index.style.transform = 'rotate(45deg)';
                fingerElements.middle.style.transform = 'rotate(45deg)';
                fingerElements.ring.style.transform = 'rotate(45deg)';
                fingerElements.pinky.style.transform = 'rotate(45deg)';
                break;
            case 'y':
                fingerElements.thumb.style.transform = 'rotate(90deg)';
                fingerElements.index.style.transform = 'rotate(90deg)';
                fingerElements.middle.style.transform = 'rotate(90deg)';
                fingerElements.ring.style.transform = 'rotate(90deg)';
                break;
            default:
                Object.values(fingerElements).forEach(el => {
                    const randomAngle = Math.floor(Math.random() * 90);
                    el.style.transform = `rotate(${randomAngle}deg)`;
                });
        }
    }
    
    updateFingers(tokens[0]);
    
    window.animationInterval = setInterval(() => {
        currentIndex++;
        
        if (currentIndex >= tokens.length) {
            clearInterval(window.animationInterval);
            setTimeout(() => {
                letterDisplay.textContent = "Done";
                document.querySelectorAll('.finger').forEach(finger => {
                    finger.style.transform = 'rotate(0deg)';
                });
            }, 1000);
            return;
        }
        
        updateFingers(tokens[currentIndex]);
    }, 2000);
}