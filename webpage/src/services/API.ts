const ARDUINO_URL = "http://192.168.100.200";

export const getMedidas = async () => {
    try {
        const response = await fetch(ARDUINO_URL);
        return await response.json();
    } catch (e) {
        return e;
    }
};
