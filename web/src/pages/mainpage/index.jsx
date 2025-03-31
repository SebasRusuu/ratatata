import React from "react";
import reactLogo from "../../assets/react.svg";
import viteLogo from "/vite.svg";
import "./index.css"; // Mantém o estilo

const MainPage = () => {
  return (
    <div className="text-center">
      {/* Logos com links */}
      <div>
        <a href="/dashboard" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a href="/controller">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>

      <h1>DashBoard - Controller</h1>
      <p className="read-the-docs">Clica no logo para abrir uma das opções</p>
    </div>
  );
};

export default MainPage;
