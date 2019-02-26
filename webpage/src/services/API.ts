import axios from "axios";

const ARDUINO_URL = "http://192.168.100.200";

const api = axios.create({
    baseURL: ARDUINO_URL,
});

export const getMedidas = async () => {
    try {
        const response = await api.get("/sensors");
        return response.data;
    } catch (e) {
        return e;
    }
};

export const sendIp = async (ip: any) => {
    try {
        const response = await api.post("/ip", ip);
        console.log(response);
        return response;
    } catch (e) {
        return e;
    }
};
