CREATE TABLE if not exists carro(
    carro_id SERIAL PRIMARY KEY not null,
    carro_rpm int not null,
    carro_bateria int not null,
    carro_tempoPercuso int not null,
    carro_sensorDistancia int not null,
    carro_velocidade int not null
);