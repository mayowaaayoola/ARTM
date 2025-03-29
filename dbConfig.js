const { Pool } = require("pg");

const itemsPool = new Pool({
  //   connectionString: process.env.DBConfigLink,
  user: "temperature_sensor_user",
  host: "dpg-cp1m48u3e1ms738p8c30-a.oregon-postgres.render.com",
  password: "NQeEy7hlvQD03nZs5YTs2M1Mbj7wM5TA",
  port: "5432",
  database: "temperature_sensor",
  ssl: {
    rejectUnauthorized: false,
  },
});
module.exports = itemsPool;
