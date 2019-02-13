import { Card, Col, Icon, Row, Statistic } from "antd";
import * as React from "react";

export interface ICardMedidaState {
    temp: number;
    hum: number;
}
export default class CardMedida extends React.Component<{}, ICardMedidaState> {
    constructor(props: any) {
        super(props);
        this.state = {
            temp: 0,
            hum: 0,
        };
        // Atualiza temperatura a cada 5s
        setInterval(this.getMedidas, 5000);
    }

    public componentDidMount() {
        this.getMedidas();
    }

    public render() {
        return (
            <Row type="flex" gutter={8} justify="space-around">
                <Col xs={24}>
                    <Card className="Card">
                        <Statistic
                            title="Temperatura"
                            decimalSeparator=","
                            value={this.state.temp}
                            prefix={<Icon type="fire" />}
                            suffix="°C"
                        />
                    </Card>
                </Col>
                <Col xs={24}>
                    <Card className="Card">
                        <Statistic
                            title="Umidade"
                            decimalSeparator=","
                            value={this.state.hum}
                            prefix={<Icon type="thunderbolt" />}
                            suffix="%"
                        />
                    </Card>
                </Col>
            </Row>
        );
    }

    private getMedidas = async () => {
        // Pega dados do servidor
        const response = await fetch("http://192.168.100.200:80");
        const json = await response.json();
        // Atualiza state
        this.setState(json);
    }
}
