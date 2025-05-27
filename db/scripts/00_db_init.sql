CREATE TABLE IF NOT EXISTS carro (
    carro_rpm               INT     NOT NULL,
    carro_bateria           INT     NOT NULL,
    carro_tempoPercuso      INT     NOT NULL,
    carro_sensorDistancia   INT     NOT NULL,
    carro_velocidade        REAL    NOT NULL,
    carro_velocidade_maxima REAL    NOT NULL DEFAULT 0
);