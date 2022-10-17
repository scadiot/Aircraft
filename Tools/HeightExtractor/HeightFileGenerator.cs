using Common;
using PlanetLib.Srtm;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace HeightExtractor
{
    internal class HeightFileGenerator
    {
        ISrtmData srtmData;
        ushort[][,] elevations;

        public HeightFileGenerator()
        {
            srtmData = new SrtmData("C:\\TMP\\aircraft\\heightmap");
        }

        public int? GetElevation(int zoom, int tileX, int tileY)
        {
            int divisor = zoom == 0 ? 1 : (int)Math.Pow(2, zoom);
            double mercatorSize = 2.0d / divisor;
            double mercatorXStart = mercatorSize * tileX - 1;
            double mercatorYStart = mercatorSize * tileY - 1;
            MercatorTools.Location location = MercatorTools.getMercatorToLatitudeLongitude(new MercatorTools.Location(mercatorXStart, mercatorYStart)); 
            return srtmData.GetElevation(new GeographicalCoordinates(location.Y, location.X));
        }

        public int[] GetElevations(int zoom, int tileX, int tileY, int gridSize)
        {
            List<int> result = new List<int>();

            int divisor = zoom == 0 ? 1 : (int)Math.Pow(2, zoom);
            double mercatorSize = 2.0d / divisor;
            double mercatorXStart = mercatorSize * tileX - 1;
            double mercatorYStart = mercatorSize * tileY - 1;
            double step = mercatorSize / gridSize;

            for (int _y = 0;_y < gridSize;_y++)
            {
                for (int _x = 0;_x < gridSize;_x++)
                {
                    double x = ((double)_x * step) + mercatorXStart;
                    double y = ((double)_y * step) + mercatorYStart;
                    MercatorTools.Location location = MercatorTools.getMercatorToLatitudeLongitude(new MercatorTools.Location(x, y));
                    int? elevation = srtmData.GetElevation(new GeographicalCoordinates(location.Y, location.X));
                    if(elevation == null)
                    {
                        elevation = 0;
                    }
                    result.Add((int)elevation);
                }
            }

            return result.ToArray();
        }

        public void GenerateFile(int zoom, int tileX, int tileY, int gridSize, string path)
        {
            string filePath = Path.Combine(path, zoom.ToString() + "_" + tileY.ToString() + "_" + tileX.ToString() + ".height");
            int[] elevations = GetElevations(zoom, tileX, tileY, gridSize);
            List<byte> data = new List<byte>();
            foreach (int elevation in elevations)
            {
                var elevationBit = BitConverter.GetBytes(elevation);
                data.AddRange(elevationBit);
            }
            File.WriteAllBytes(filePath, data.ToArray());
        }

        public void GenerateElevationImage(double latitudeMin, double longitutdeMin, double latitudeMax, double longitudeMax, int size, string imagePath)
        {
            var values = new List<byte>();

            double latitudeStep = (latitudeMax - latitudeMin) / size;
            double longitudeStep = (longitutdeMin - longitudeMax) / size;
            for(int x = 0;x < size; x++)
            {
                for (int y = 0; y < size; y++)
                {
                    int? elevationNullable = srtmData.GetElevation(new GeographicalCoordinates(latitudeMin + (y * latitudeStep), latitudeMax + (x * longitudeStep)));
                    ushort elevation = elevationNullable != null ? (ushort)elevationNullable : (ushort)0;
                    values.AddRange(BitConverter.GetBytes(elevation));
                }
            }
        }

        public void LoadData(int startZoom, int tileX, int tileY, int gridSize, string path)
        {
            int maxZoom = 16;
            int levelCount = maxZoom - (startZoom - 1);
            elevations = new ushort[maxZoom][,];

            for(int currentZoom = maxZoom; currentZoom >= startZoom; currentZoom--)
            {
                int indexZoom = currentZoom - 1;
                //int size = (int)Math.Pow(2, currentZoom - (startZoom - 1)) * gridSize;
                //elevations[indexZoom] = new ushort[size + 1, size + 1];

                int startTileX = tileX;
                int startTileY = tileY;
                int endTileX = tileX + 1;
                int endTileY = tileY + 1;
                for (int i = startZoom + 1; i <= currentZoom; i++)
                {
                    startTileX *= 2;
                    startTileY *= 2;
                    endTileX *= 2;
                    endTileY *= 2;
                }
                int size = (endTileY - startTileY);
                elevations[indexZoom] = new ushort[size * gridSize, size * gridSize];
                List<byte> fileData = new List<byte>();
                fileData.AddRange(BitConverter.GetBytes(currentZoom));
                fileData.AddRange(BitConverter.GetBytes(startTileX));
                fileData.AddRange(BitConverter.GetBytes(startTileY));
                fileData.AddRange(BitConverter.GetBytes(endTileX - 1));
                fileData.AddRange(BitConverter.GetBytes(endTileY - 1));
                int elevationCount = (int)Math.Pow(size * gridSize, 2);
                fileData.AddRange(BitConverter.GetBytes(elevationCount));
                for (int currentTileY = startTileY; currentTileY < endTileY; currentTileY++)
                {
                    for (int y = 0; y < gridSize; y++)
                    {
                        for (int currentTileX = startTileX; currentTileX < endTileX; currentTileX++)
                        {
                            double offsetY = (double)y / gridSize;
                            for (int x = 0; x < gridSize; x++)
                            {
                                double offsetX = (double)x / gridSize;
                                int indexX = (currentTileX - startTileX) * gridSize + x;
                                int indexY = (currentTileY - startTileY) * gridSize + y;
                                ushort elevation = 0;
                                //if (currentZoom == maxZoom)
                                {
                                    var location = MercatorTools.getTilePositionToLatitudeLongitude(currentZoom, currentTileX, currentTileY, offsetX, offsetY);
                                    var result = srtmData.GetElevation(new GeographicalCoordinates(location.Y, location.X));
                                    if(result != null)
                                    {
                                        elevation = (ushort)result;
                                    }
                                }
                                //else
                                //{
                                //    int targetX = indexX * 2;
                                //    int targetY = indexY * 2;
                                //    elevation = (ushort)((elevations[indexZoom + 1][targetX, targetY] + elevations[indexZoom + 1][targetX + 1, targetY] + elevations[indexZoom + 1][targetX, targetY + 1] + elevations[indexZoom + 1][targetX + 1, targetY + 1]) / 4);
                                //}
                                elevations[indexZoom][indexX, indexY] = elevation;
                                fileData.AddRange(BitConverter.GetBytes(elevation));
                            }
                        }
                    }
                }
                File.WriteAllBytes(Path.Combine(path, (indexZoom + 1).ToString() + ".height"), fileData.ToArray());
            }

            
            Console.WriteLine("totalSize: ");
        }

        //public void loadData(int startZoom, int gridSize, string path)
        //{
        //    //getMercatorToLatitudeLongitude()
        //    //double latitudeMin, double longitutdeMin, double latitudeMax, double longitudeMax;
        //    int levelCount = 16 - (startZoom - 1);
        //    int size = 2048;
        //    double latitudeStep = 0;// (latitudeMax - latitudeMin) / size;
        //    double longitudeStep = 0;//(longitutdeMin - longitudeMax) / size;
        //    elevations = new ushort[levelCount][,];
        //    elevations[0] = new ushort[size, size];
        //    for (int x = 0; x < size; x++)
        //    {
        //        for (int y = 0; y < size; y++)
        //        {
        //            int? elevationNullable = srtmData.GetElevation(new GeographicalCoordinates(latitudeMin + (y * latitudeStep), latitudeMax + (x * longitudeStep)));
        //            ushort elevation = elevationNullable != null ? (ushort)elevationNullable : (ushort)0;
        //            elevations[0][x, y] = elevation;
        //        }
        //    }
        //
        //    for(int i = 1;i < levelCount; i++)
        //    {
        //        size = size / 2;
        //        elevations[i] = new ushort[size, size];
        //        for (int x = 0; x < size; x++)
        //        {
        //            for (int y = 0; y < size; y++)
        //            {
        //                int targetX = x * 2;
        //                int targetY = y * 2;
        //                int elevation = (elevations[i - 1][targetX, targetY] + elevations[i - 1][targetX + 1, targetY] + elevations[i - 1][targetX, targetY + 1] + elevations[i - 1][targetX + 1, targetY + 1]) / 4;
        //                elevations[i][x, y] = (ushort)elevation;
        //            }
        //        }
        //    }
        //
        //    for (int i = 0; i < levelCount; i++)
        //    {
        //        List<byte> data = new List<byte>();
        //        int length = elevations[i].GetLength(0);
        //        for (int y = 0; y < length; y++)
        //        {
        //            for (int x = 0; x < length; x++)
        //            {
        //                data.AddRange(BitConverter.GetBytes(elevations[i][x, y]));
        //            }
        //        }
        //        File.WriteAllBytes(Path.Combine(path, (16 - i).ToString() + ".elevation"), data.ToArray());
        //    }
        //    Console.WriteLine("ok");
        //}

    }
}
