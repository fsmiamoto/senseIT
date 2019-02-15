import * as React from "react";

interface IConfigProps {
    default?: boolean;
    path: string;
}

export default class Config extends React.Component<IConfigProps, {}> {
    public render() {
        return <h1>Configurações</h1>;
    }
}
