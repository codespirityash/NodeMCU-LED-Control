const ip = "192.168.29.226"; 

const onButton = document.getElementById("onButton") as HTMLButtonElement;
const offButton = document.getElementById("offButton") as HTMLButtonElement;
const statusText = document.getElementById("status") as HTMLParagraphElement;

async function toggleLED(state: boolean) {
    try {
        const response = await fetch(`http://${ip}/LED=${state ? 'ON' : 'OFF'}`);
        const data = await response.json();
        
        if (data.led) {
            statusText.innerText = "LED is ON";
        } else {
            statusText.innerText = "LED is OFF";
        }
    } catch (error) {
        console.error("Error:", error);
        statusText.innerText = "Error connecting to NodeMCU";
    }
}

onButton.addEventListener("click", () => toggleLED(true));
offButton.addEventListener("click", () => toggleLED(false));
