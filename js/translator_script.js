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