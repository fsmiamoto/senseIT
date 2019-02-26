import * as React from "react";

import { Button, Col, Row } from "antd";
import moment from "moment";
import CardsMedidas from "../../components/CardsMedidas/CardsMedidas";
import { getMedidas, sendIp } from "../../services/API";

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
            hum: 0,
            humPoints: [],
            temp: 0,
            tempPoints: [],
        };
        this.atualizaMedidas();
        setInterval(this.atualizaMedidas, 5000);
    }

    public render() {
        return (
            <>
                <Row>
                    <h1>Olá, Fulano!</h1>
                </Row>
                <Row>
                    <Col span={12}>
                        <CardsMedidas
                            temp={this.state.temp}
                            hum={this.state.hum}
                        />
                    </Col>
                </Row>
                <Row>
                    <Button onClick={() => sendIp({ ip: "192.168.1.90" })}>
                        Clique aqui!
                    </Button>
                </Row>
            </>
        );
    }

    private atualizaMedidas = async () => {
        const { temp, hum } = await getMedidas();

        // Atualiza array de pontos
        const agora = moment();
        this.state.tempPoints.push({ temp, time: agora });
        this.state.humPoints.push({ hum, time: agora });

        // Atualiza estado com valores atuais
        this.setState({ temp, hum });
    }
}
