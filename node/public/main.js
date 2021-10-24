var map = new ol.Map({
  target: "map",
  layers: [
    new ol.layer.Tile({
      source: new ol.source.OSM(),
    }),
  ],

  view: new ol.View({
    center: ol.proj.fromLonLat([44.0, 56.33]),
    zoom: 12,
  }),
});

const addMapPoint = (lat, lng, label, offsetY) => {
  iconStyle = [
    new ol.style.Style({
      image: new ol.style.Icon({
        anchor: [0.5, 1],
        anchorXUnits: "fraction",
        anchorYUnits: "fraction",
        src: "img/marker.png",
        scale: 0.1,
      }),
    }),
    new ol.style.Style({
      text: new ol.style.Text({
        text: label,
        offsetY: offsetY,
        scale: 1.5,
        fill: new ol.style.Fill({
          color: "#black",
        }),
      }),
    }),
  ];
  const vectorLayer = new ol.layer.Vector({
    source: new ol.source.Vector({
      features: [
        new ol.Feature({
          geometry: new ol.geom.Point(
            ol.proj.transform(
              [parseFloat(lng), parseFloat(lat)],
              "EPSG:4326",
              "EPSG:3857"
            )
          ),
        }),
      ],
    }),
    style: iconStyle,
  });
  map.addLayer(vectorLayer);
};

addMapPoint(56.332871, 44.038892, "test", 0);

map.on("singleclick", function (evt) {
  var pos = ol.proj.transform(evt.coordinate, "EPSG:3857", "EPSG:4326");

  createRound(pos[1], pos[0], 3);
  //addMapPoint(pos[1], pos[0], 'test', 0)
});

var createRound = (lat, lon, rad) => {
  var centerLongitudeLatitude = ol.proj.fromLonLat([lon, lat]);

  var layer = new ol.layer.Vector({
    source: new ol.source.Vector({
      projection: "EPSG:4326",
      // radius = 4000 meters
      features: [
        new ol.Feature(new ol.geom.Circle(centerLongitudeLatitude, rad)),
      ],
    }),
    style: [
      new ol.style.Style({
        stroke: new ol.style.Stroke({
          color: "blue",
          width: 3,
        }),
        fill: new ol.style.Fill({
          color: "rgba(0, 0, 255, 0.1)",
        }),
      }),
    ],
  });
  map.addLayer(layer);
};
