import { faThermometerHalf, faTint } from "@fortawesome/free-solid-svg-icons";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { Card, Col, Row, Statistic } from "antd";
import * as React from "react";

interface ICardsMedidasProps {
    temp: number;
    hum: number;
}
const CardsMedidas = (props: ICardsMedidasProps) => {
    return (
        <Row type="flex" gutter={8} justify="space-around">
            <Col xs={24}>
                <Card className="Card">
                    <Statistic
                        title="Temperatura"
                        decimalSeparator=","
                        value={props.temp}
                        prefix={
                            <FontAwesomeIcon
                                icon={faThermometerHalf}
                                style={{ marginRight: "10px" }}
                            />
                        }
                        suffix="°C"
                    />
                </Card>
            </Col>
            <Col xs={24}>
                <Card className="Card">
                    <Statistic
                        title="Umidade"
                        decimalSeparator=","
                        value={props.hum}
                        prefix={
                            <FontAwesomeIcon
                                icon={faTint}
                                style={{ marginRight: "10px" }}
                            />
                        }
                        suffix="%"
                    />
                </Card>
            </Col>
        </Row>
    );
};

export default CardsMedidas;
