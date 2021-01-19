const express = require("express");
const cors = require('cors')
const multer = require('multer')
const fs = require('fs')
const app = express();
const parser = require('xml2json')
const format = require('js2xmlparser')

app.use(cors());

let storage = multer.diskStorage({
    destination: (req, file, callback) => {
        callback(null, 'uploads/');
    },
    filename: (req, file, callback) => {
        let filename = 'map.tmx'
        callback(null, filename);
    }
});

let uploadFile = multer({ storage: storage })

app.get("/", function (req, res) {
    res.send("Welcome to this api");
})

let gridConfig = {
    cellwidth: 0,
    cellheight: 0,
    widthInTiles: 0,
    heightInTiles: 0
}

function generateGridData(objects, config)
{
    let cellwidth = config.cellwidth
    let cellheight = config.cellheight
    let width = config.widthInTiles
    let height = config.heightInTiles

    let result = []
    objects.forEach(obj => {
        let index = {
            x: Math.trunc(obj.x / cellwidth),
            y: Math.trunc(obj.y / cellheight)
        }

        if (index.x < 0 || index.x >= width || index.y < 0 || index.y >= height) index = undefined

        result.push({
            id: obj.id,
            cell: index
        })
    })

    return result
}

function dataToXml(data) 
{
    var xml = '<?xml version="1.0" encoding="utf-8"?>\n'
    xml += '<grid>\n'
    xml += `  <config cellwidth="${data.gridconfig.cellwidth}" cellheight="${data.gridconfig.cellheight}" width="${data.gridconfig.widthInTiles}" height="${data.gridconfig.heightInTiles}"/>\n`
    xml += '  <data>\n'

    data.data.forEach(d => {
        var grid = d.grid
        xml += `    <group id="${d.id}" name="${d.name}">\n`

        grid.forEach(g => {
            xml += `        <object id="${g.id}" cellx="${g.cell.x}" celly="${g.cell.y}"/>\n`
        })

        xml += '    </group>\n'
    })

    xml += '  </data>\n'
    xml += '</grid>\n'

    return xml
}

app.post("/", uploadFile.single('file'), function (req, res) {
    // console.log(req.files.file.data)
    fs.readFile(`${__dirname}/uploads/map.tmx`, 'utf8', (err, data) => {
        console.log('----')
        var jsonobj = JSON.parse(parser.toJson(data))
        var map = jsonobj.map

        gridConfig.cellwidth = req.body.cellwidth
        gridConfig.cellheight = req.body.cellheight
        gridConfig.widthInTiles = Math.ceil(map.width * map.tilewidth / gridConfig.cellwidth)
        gridConfig.heightInTiles = Math.ceil(map.height * map.tileheight / gridConfig.cellheight)

        // console.log(gridConfig)

        gridData = []
        map.objectgroup.forEach(group => {
            // console.log(group)
            gridData.push({
                id: group.id,
                name: group.name,
                grid: generateGridData(group.object, gridConfig)
            })
        })

        var final = {
            gridconfig: gridConfig,
            data: gridData
        }

        var xmlFinal = dataToXml(final)
        console.log(xmlFinal)
        fs.writeFileSync(`${__dirname}/uploads/grid-result.xml`, xmlFinal)
        fs.writeFileSync(`${__dirname}/uploads/grid-result.json`, JSON.stringify(final, undefined, 2))

        // res.json(jsonobj.map)
        res.json({
            gridconfig: gridConfig,
            data: gridData
        })
    })
})

app.listen(process.env.PORT || 3000, () => {
    console.log("Listening on port 3000")
})