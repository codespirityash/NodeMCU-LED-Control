document.addEventListener("DOMContentLoaded", function () {
  const slider = document.getElementById("brightness");
  const displayValue = document.getElementById("brightnessValue");

  const espIP = "http://192.168.1.100";

  slider.addEventListener("input", function () {
    displayValue.textContent = slider.value;
    fetch(`${espIP}/setBrightness?value=${slider.value}`, { mode: "cors" })
      .then((response) => response.text())
      .then((data) => console.log("Response:", data))
      .catch((error) => console.error("Fetch Error:", error));
  });
});
