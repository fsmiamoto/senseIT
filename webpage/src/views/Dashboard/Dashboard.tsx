import * as React from "react";

import { Col, Row } from "antd";
import moment from "moment";
import CardMedida from "../../components/CardMedida/CardMedida";
import { Grafico } from "../../components/Grafico/Grafico";

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
        // TODO: Implementar gráfico
        const grafData = this.state.tempPoints.map((point) => ({
            time: point.time.format("HH:mm"),
            amt: point.temp,
        }));
        return (
            <>
                <Row>
                    <h1>Olá, Fulano!</h1>
                </Row>
                <Row>
                    <Col span={12}>
                        <CardMedida />
                    </Col>
                </Row>
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
