using Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace TilesScraper
{
    internal class TilesDownloader
    {
        public static void DownloadFromZoomToZoom2(int from_zoom, int start_x, int start_y, int end_x, int end_y, int to_zoom, string destination)
        {
            for(int x = start_x;x <= end_x;x++)
            {
                for (int y = start_y; y <= end_y; y++)
                {
                    DownloadFromZoomToZoom(from_zoom, x, y, to_zoom, destination);
                }
            }
        }

        public static void DownloadFromZoomToZoom(int from_zoom, int x, int y, int to_zoom, string destination)
        {
            int currentZoom = from_zoom + 1;
            int fromX = x * 2;
            int fromY = y * 2;
            for (int _y = fromY; _y < fromY + 2; _y++)
            {
                for (int _x = fromX; _x < fromX + 2; _x++)
                {
                    Thread.Sleep(100);
                    string imagePath = Path.Combine(destination, currentZoom.ToString() + "_" + _y.ToString() + "_" + _x.ToString() + ".png");
                    if(!File.Exists(imagePath))
                    {
                        DownloadTile(_x, _y, currentZoom, imagePath);
                    }
                    if (currentZoom != to_zoom)
                    {
                        DownloadFromZoomToZoom(from_zoom + 1, _x, _y, to_zoom, destination);
                    }
                }
            }
            
        }

        public static void DownloadAllMiddleZoom(int zoom, string destination)
        {
            int maxTileIndex = MercatorTools.MaxTileIndex(zoom);
            int maxTileIndexQuarter = maxTileIndex / 4;
            int start = maxTileIndexQuarter;
            int end = start + (maxTileIndexQuarter * 2);
            for (int y = start; y < end; y++)
            {
                for (int x = 0; x < maxTileIndex; x++)
                {
                    Thread.Sleep(1000);
                    string imagePath = Path.Combine(destination, zoom.ToString() + "_" + y.ToString() + "_" + x.ToString() + ".png");
                    DownloadTile(x, y, zoom, imagePath);
                }
            }
        }

        public static void DownloadAllZoom(int zoom, string destination)
        {
            int maxTileIndex = MercatorTools.MaxTileIndex(zoom);
            DownloadAllTiles(zoom, 0, maxTileIndex, 0, maxTileIndex, destination);
        }

        public static void DownloadAllTiles(int zoom, int minX, int maxX, int minY, int maxY, string destination)
        {
            for (int y = minY; y < maxY; y++)
            {
                for (int x = minX; x < maxX;x++)
                {
                    Thread.Sleep(1000);
                    string imagePath = Path.Combine(destination, zoom.ToString()+"_"+y.ToString()+"_"+x.ToString() + ".png");
                    DownloadTile(x, y, zoom, imagePath);
                }
            }
        }

        public static void DownloadTile(int x, int y, int zoom, string imagePath)
        {
            using (WebClient myWebClient = new WebClient())
            {
                try
                {
                    //string url = "https://a.tile.openstreetmap.de/" + tilesLoaderInfo.zoom + "/" + tilesLoaderInfo.x + "/" + tilesLoaderInfo.y + ".png";
                    //string url = "https://wxs.ign.fr/essentiels/geoportail/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=ORTHOIMAGERY.ORTHOPHOTOS&TILEMATRIXSET=PM&TILEMATRIX=" + tilesLoaderInfo.zoom + "&TILECOL=" + tilesLoaderInfo.x + "&TILEROW=" + tilesLoaderInfo.y + "&STYLE=normal&FORMAT=image/jpeg";
                    string url = "http://ak.dynamic.t0.tiles.virtualearth.net/comp/ch/" + TileXYToQuadKey(x, y, zoom) + "?mkt=en-US&it=A&og=1903&n=z";
                    ServicePointManager.Expect100Continue = true;
                    ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;
                    myWebClient.DownloadFile(url, imagePath);
                }
                catch (WebException webException)
                {
                    File.Delete(imagePath);
                }
            }
        }

        public static string TileXYToQuadKey(int tileX, int tileY, int levelOfDetail)
        {
            StringBuilder quadKey = new StringBuilder();
            for (int i = levelOfDetail; i > 0; i--)
            {
                char digit = '0';
                int mask = 1 << (i - 1);
                if ((tileX & mask) != 0)
                {
                    digit++;
                }
                if ((tileY & mask) != 0)
                {
                    digit++;
                    digit++;
                }
                quadKey.Append(digit);
            }
            return quadKey.ToString();
        }
    }
}
