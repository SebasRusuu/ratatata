import React from "react";
import { Link } from "react-router-dom"; // Importa o Link do React Router
import "bootstrap/dist/css/bootstrap.min.css";

const enviarComando = (comando) => {
  fetch("http://localhost:1880/enviar-comando", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ comando })
  })
  .then(response => response.json())
  .then(data => console.log('Resposta Node-RED:', data))
  .catch(error => console.error('Erro no fetch:', error));
};



const Controller = () => {
  return (
    <div className="d-flex flex-column align-items-center gap-2 mt-4">
      <button className="btn btn-primary" onClick={() => enviarComando("frente")}>▲ Frente</button>
      <div className="d-flex gap-2">
        <button className="btn btn-secondary" onClick={() => enviarComando("esquerda")}>◀ Esquerda</button>
        <button className="btn btn-secondary" onClick={() => enviarComando("direita")}>▶ Direita</button>
      </div>
      <button className="btn btn-danger" onClick={() => enviarComando("tras")}>▼ Trás</button>

      {/* Botão de Voltar para a Página Principal */}
      <Link to="/" className="btn btn-warning mt-3">
        ⬅ Voltar para a Página Inicial
      </Link>
    </div>
  );
};

export default Controller;
