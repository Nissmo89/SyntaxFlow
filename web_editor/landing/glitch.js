// Injects an SVG filter into the document to create a real DOM glitch effect
const svgNS = "http://www.w3.org/2000/svg";

const svg = document.createElementNS(svgNS, "svg");
svg.style.position = "absolute";
svg.style.width = "0";
svg.style.height = "0";
svg.style.pointerEvents = "none";

svg.innerHTML = `
  <defs>
    <filter id="dom-glitch" x="-20%" y="-20%" width="140%" height="140%">
      <feTurbulence id="glitch-turbulence" type="fractalNoise" baseFrequency="0.01 0.5" numOctaves="1" result="noise" />
      <feColorMatrix type="matrix" values="1 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 5 -2" in="noise" result="coloredNoise" />
      
      <!-- Displace the actual DOM elements -->
      <feDisplacementMap id="glitch-displacement" in="SourceGraphic" in2="coloredNoise" scale="0" xChannelSelector="R" yChannelSelector="G" result="displaced" />
      
      <!-- RGB Split -->
      <feOffset id="glitch-offset-r" in="displaced" dx="0" dy="0" result="red-shift" />
      <feOffset id="glitch-offset-b" in="displaced" dx="0" dy="0" result="blue-shift" />
      <feOffset in="displaced" dx="0" dy="0" result="green-shift" />
      
      <feColorMatrix in="red-shift" type="matrix" values="1 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 1 0" result="red" />
      <feColorMatrix in="green-shift" type="matrix" values="0 0 0 0 0  0 1 0 0 0  0 0 0 0 0  0 0 0 1 0" result="green" />
      <feColorMatrix in="blue-shift" type="matrix" values="0 0 0 0 0  0 0 0 0 0  0 0 1 0 0  0 0 0 1 0" result="blue" />
      
      <feBlend mode="screen" in="red" in2="green" result="rg" />
      <feBlend mode="screen" in="rg" in2="blue" result="rgb" />
    </filter>
  </defs>
`;

document.body.appendChild(svg);

let glitchActive = false;
let glitchTimer = null;
let animFrame = null;

function animateGlitch() {
    if (!glitchActive) return;
    
    const turb = document.getElementById("glitch-turbulence");
    const disp = document.getElementById("glitch-displacement");
    const offR = document.getElementById("glitch-offset-r");
    const offB = document.getElementById("glitch-offset-b");
    
    // Rapidly randomize noise frequency to create blocky bands
    const freqX = Math.random() * 0.1 + 0.005;
    const freqY = Math.random() * 0.5 + 0.05;
    turb.setAttribute("baseFrequency", freqX + " " + freqY);
    
    // Randomize displacement scale (block intensity)
    const scale = Math.random() * 25 + 5;
    disp.setAttribute("scale", scale);
    
    // Randomize RGB split offset
    const rgbShiftX = (Math.random() - 0.5) * 15;
    const rgbShiftY = (Math.random() - 0.5) * 5;
    offR.setAttribute("dx", rgbShiftX);
    offR.setAttribute("dy", rgbShiftY);
    offB.setAttribute("dx", -rgbShiftX);
    offB.setAttribute("dy", -rgbShiftY);
    
    // Re-trigger roughly every 30-50ms for that chaotic digital stutter
    setTimeout(() => {
        if (glitchActive) animFrame = requestAnimationFrame(animateGlitch);
    }, 30 + Math.random() * 20);
}

window.triggerGlitchTransition = function(targetElement, onMidpoint) {
    if (glitchActive) return;
    
    glitchActive = true;
    targetElement.style.filter = "url(#dom-glitch)";
    animateGlitch();
    
    // At midpoint, we change the theme
    setTimeout(() => {
        onMidpoint();
        
        // After a bit more glitching, end the effect
        setTimeout(() => {
            glitchActive = false;
            targetElement.style.filter = "none";
            
            // reset attributes to 0
            const disp = document.getElementById("glitch-displacement");
            disp.setAttribute("scale", "0");
            document.getElementById("glitch-offset-r").setAttribute("dx", "0");
            document.getElementById("glitch-offset-b").setAttribute("dx", "0");
            
        }, 150);
        
    }, 250);
};
