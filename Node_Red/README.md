# node-Red README

## Verwendung
vor importieren der flows in der Weboberfläche muss noch in der `settings.js` unter `/home/pi/.node-red/settings.js` folgende zeile auskommentiert werden bzw auf `localfilesystem` geändert werden damit Variablen nach Neustart noch vorhanden sind
```js
contextStorage: {
        default: {
            module:"localfilesystem"
        },
    },
```
