const { Pool, Client } = require('pg')
const express = require('express')
const app = express()
const port = process.env.PORT || 3000;

app.get('/', (req, res) => {
    const client = new Client({
        user: 'vzmvmwyepoqpvl',
        host: 'ec2-52-0-114-209.compute-1.amazonaws.com',
        database: 'dbkbcvt12pj6ek',
        password: 'd236c1885cfb1694cfab0b91210df1930a6282466d145d2f179c4a24a5925c23',
        port: 5432
    });
    try{
        client.connect();
    }catch (e){
        console.log(e);
        console.error(e);
    }
    client.query('SSELECT DISTINCT * FROM vws_geo_db WHERE water_level > 100', (err, res) => {
        console.log(err, res)
        client.end()
    })
})

app.listen(port, () => {
    console.log("Server listening on port " + port);
});