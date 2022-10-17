namespace Common
{
    public class MercatorTools
    {
        public class Location
        {
            public double X { get; set; }
            public double Y { get; set; }

            public Location()
            {

            }

            public Location(double x, double y)
            {
                X = x;Y = y;
            }
        }

        public static int MaxTileIndex(int zoom)
        {
            if(zoom == 0)
            {
                return 1;
            }
            else if (zoom == 1)
            {
                return 2;
            }
            return (int)Math.Pow(2, zoom);
        }

        public static Location getMercatorToLatitudeLongitude(Location position)
        {
            return new Location()
            {
                X = position.X * 180,
                Y = (180d / Math.PI * (2d * Math.Atan(Math.Exp(position.Y * Math.PI)) - Math.PI / 2)) * -1d
            };
        }

        public static Location getTilePositionToLatitudeLongitude(int zoom, int tileX, int tileY, double offsetX, double offsetY)
        {
            int divisor = zoom == 0 ? 1 : (int)Math.Pow(2, zoom);
            double mercatorSize = 2.0d / divisor;
            double mercatorXStart = (mercatorSize * tileX - 1) + (mercatorSize * offsetX);
            double mercatorYStart = (mercatorSize * tileY - 1) + (mercatorSize * offsetY);
            return MercatorTools.getMercatorToLatitudeLongitude(new MercatorTools.Location(mercatorXStart, mercatorYStart));
        }
    }
}