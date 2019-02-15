import * as React from "react";
import { CartesianGrid, Line, LineChart, XAxis, YAxis } from "recharts";

export const Grafico = (data: any) => {
    console.log(data.data);
    return (
        <LineChart
            width={600}
            height={300}
            data={data.data}
            margin={{
                top: 5,
                right: 30,
                left: 20,
                bottom: 5,
            }}
        >
            <Line
                type="monotone"
                dataKey="time"
                stroke="#8884d8"
                name="Temperatura"
            />
            <CartesianGrid stroke="#ccc" />
            <XAxis dataKey="time" />
            <YAxis dataKey="amt" />
        </LineChart>
    );
};
