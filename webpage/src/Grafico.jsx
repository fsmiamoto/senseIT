import * as React from "react";
import {
    HorizontalGridLines,
    LineSeries,
    XAxis,
    XYPlot,
    YAxis
} from "react-vis";

export const Grafico = data => (
    <XYPlot width={300} height={300}>
        <HorizontalGridLines />
        <XAxis />
        <YAxis />
    </XYPlot>
);
