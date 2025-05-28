CREATE TABLE carro (
  id SERIAL PRIMARY KEY,
  carro_bateria          INT     NOT NULL,
  carro_tempoPercurso    INT     NOT NULL,
  carro_sensorDistancia  INT     NOT NULL,
  -- acelerómetro (valores máximos registados)
  ax_max     REAL    NOT NULL DEFAULT 0,
  ay_max     REAL    NOT NULL DEFAULT 0,
  az_max     REAL    NOT NULL DEFAULT 0,
  -- giroscópio (valores máximos registados)
  gx_max     REAL    NOT NULL DEFAULT 0,
  gy_max     REAL    NOT NULL DEFAULT 0,
  gz_max     REAL    NOT NULL DEFAULT 0,
  -- força G (valor máximo registado)
  gforce_max REAL    NOT NULL DEFAULT 0,
  criado_em  TIMESTAMPTZ NOT NULL DEFAULT now()
);
