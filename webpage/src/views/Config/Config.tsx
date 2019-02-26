import { Button } from "antd";
import * as React from "react";

import { sendIp } from "../../services/API";

interface IConfigProps {
    default?: boolean;
    path: string;
}

export default class Config extends React.Component<IConfigProps, {}> {
    public render() {
        return (
            <>
                <h1>Configurações</h1>
                <Button onClick={() => sendIp({ ip: "192.168.1.90" })}>
                    Enviar IP
                </Button>
            </>
        );
    }
}
