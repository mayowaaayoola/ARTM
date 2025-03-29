const express = require("express");
const itemsPool = require("./dbConfig");
const cors = require("cors");

const dotenv = require("dotenv");
dotenv.config();

const app = express();
app.use(express.json());
app.use(cors());


app.get("/", (req, res) => {
  res.send("Welcome!");
});

// Handle POST request from ESP32
app.post("/api/save", async (req, res) => {
  const { temperature } = req.body;

  try {
    const client = await itemsPool.connect();
    const result = await client.query(
      "INSERT INTO sensor_data (temperature) VALUES ($1)",
      [temperature]
    );
    client.release();

    res.json({
      message: "successfully added new item",
      item: result.rows,
    });
  } catch (error) {
    console.error("Error executing query", error);
    res.status(500).send(error.message);
  }
});

app.get("/api/items", async (req, res) => {
  try {
    const allItems = await itemsPool.query("SELECT * FROM sensor_data");
    res.json({ allItems: allItems.rows });
  } catch (error) {
    console.log(error);
    res.status(500).send(error.message);
  }
});

// Start server
app.listen(5000, () => {
  console.log(`Server is running on port 5000`);
});
