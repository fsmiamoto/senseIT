import { Icon, Layout, Menu } from "antd";
import * as React from "react";

import moment from "moment";
import CardMedida from "../CardMedida/CardMedida";
import { Grafico } from "../Grafico";

const server = "http://192.168.100.200:80";

interface IDashboardProps {
    default: boolean;
    path: string;
}

interface IDashboardState {
    temp: number;
    hum: number;
    tempPoints: Array<{ temp: number; time: moment.Moment }>;
    humPoints: Array<{ hum: number; time: moment.Moment }>;
}

export default class Dashboard extends React.Component<
    IDashboardProps,
    IDashboardState
> {
    constructor(props: IDashboardProps) {
        super(props);
        this.state = {
            temp: 0,
            hum: 0,
            tempPoints: [],
            humPoints: [],
        };
        setInterval(this.getMedidas, 5000);
    }

    public render() {
        const grafData = this.state.tempPoints.map((point, index) => ({
            x: point.temp,
            y: index,
        }));
        return (
            <>
                <h1>Olá, Fulano!</h1>
                <CardMedida />
                <Grafico data={grafData} />
            </>
        );
    }

    private getMedidas = async () => {
        // Pega dados do servidor
        const response = await fetch(server);
        // Obtém temp e hum do JSON
        const { temp, hum } = await response.json();
        // Atualiza vetor de pontos
        const agora = moment();
        this.state.tempPoints.push({ temp, time: agora });
        this.state.humPoints.push({ hum, time: agora });
        // Atualiza estado com valores atuais
        this.setState({ temp, hum });
    }
}
