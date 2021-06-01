const { Pool, Client } = require('pg')
const express = require('express')
const app = express()
const port = process.env.PORT || 3000;

app.get('/', async (req, res) => {
    const client = new Client({
        user: 'vzmvmwyepoqpvl',
        host: 'ec2-52-0-114-209.compute-1.amazonaws.com',
        database: 'dbkbcvt12pj6ek',
        password: 'd236c1885cfb1694cfab0b91210df1930a6282466d145d2f179c4a24a5925c23',
        port: 5432,
        ssl: { rejectUnauthorized: false }
    });
    try{
        client.connect();
    }catch (e){
        console.log(e);
        console.error(e);
    }
    result = await getLocationData(client);
    console.log("result data", result);
    res.send(result)
})

async function getLocationData(client) {
    let response;
    try {
      response = await client.query('SELECT DISTINCT * FROM vws_geo_db WHERE water_level > 100');
      console.log(response)
      return response.rows;
    } catch (error) {
      // handle error
    }
  }

app.listen(port, () => {
    console.log("Server listening on port " + port);
});