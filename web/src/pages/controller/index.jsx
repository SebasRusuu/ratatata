import React from "react";
import { Link } from "react-router-dom"; // Importa o Link do React Router
import "bootstrap/dist/css/bootstrap.min.css";

const Controller = () => {
  return (
    <div className="d-flex flex-column align-items-center gap-2 mt-4">
      <button className="btn btn-primary">▲ Frente</button>
      <div className="d-flex gap-2">
        <button className="btn btn-secondary">◀ Esquerda</button>
        <button className="btn btn-secondary">▶ Direita</button>
      </div>
      <button className="btn btn-danger">▼ Trás</button>

      {/* Botão de Voltar para a Página Principal */}
      <Link to="/" className="btn btn-warning mt-3">
        ⬅ Voltar para a Página Inicial
      </Link>
    </div>
  );
};

export default Controller;
