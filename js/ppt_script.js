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