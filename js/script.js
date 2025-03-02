function isInViewport(element) {
    const rect = element.getBoundingClientRect();
    return (
        rect.top >= 0 &&
        rect.left >= 0 &&
        rect.bottom <= (window.innerHeight || document.documentElement.clientHeight) &&
        rect.right <= (window.innerWidth || document.documentElement.clientWidth)
    );
}

function handleScroll() {
    const projectCards = document.querySelectorAll('.project-card');
    projectCards.forEach(card => {
        if (isInViewport(card)) {
            card.classList.add('animate-card');
        }
    });
}

window.addEventListener('scroll', handleScroll);

handleScroll();

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