import { Link, Router } from "@reach/router";
import { Icon, Layout, Menu } from "antd";
import * as React from "react";
import "./App.scss";

// Views
import Config from "./Config/Config";
import Dashboard from "./Dashboard/Dashboard";
import { Logo } from "./style";

import logoImg from "./logo.png";

const { Content, Sider } = Layout;

export default class App extends React.Component<{}, {}> {
    public render() {
        return (
            <Layout style={{ minHeight: "100vh" }}>
                <Sider breakpoint="lg" collapsedWidth="0">
                    <Logo src={logoImg} alt="Logo" />
                    <Menu
                        theme="dark"
                        mode="inline"
                        defaultSelectedKeys={["1"]}
                    >
                        <Menu.Item key="1">
                            <Link to="/dashboard">
                                <Icon type="dashboard" />
                                Dashboard
                            </Link>
                        </Menu.Item>
                        <Menu.Item key="2">
                            <Link to="/config">
                                <Icon type="setting" />
                                Configurações
                            </Link>
                        </Menu.Item>
                    </Menu>
                </Sider>
                <Layout>
                    <Content
                        className="card-shadow"
                        style={{
                            background: "#fff",
                            margin: "16px",
                            minHeight: 280,
                            padding: 24,
                        }}
                    >
                        <Router>
                            <Dashboard default={true} path="/dashboard" />
                            <Config path="/config" />
                        </Router>
                    </Content>
                </Layout>
            </Layout>
        );
    }
}
