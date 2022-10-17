#region MIT License
// MIT License
// Copyright (c) 2012 Alpine Chough Software.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.	
#endregion

using System;
using System.IO;

namespace PlanetLib.Srtm
{
	/// <summary>
	/// SRTM data cell.
	/// </summary>
	/// <exception cref='FileNotFoundException'>
	/// Is thrown when a file path argument specifies a file that does not exist.
	/// </exception>
	/// <exception cref='ArgumentException'>
	/// Is thrown when an argument passed to a method is invalid.
	/// </exception>
	/// <exception cref='ArgumentOutOfRangeException'>
	/// Is thrown when an argument passed to a method is invalid because it is outside the allowable range of values as
	/// specified by the method.
	/// </exception>
	public class SrtmDataCell
	{
		#region Lifecycle

		/// <summary>
		/// Initializes a new instance of the <see cref="Alpinechough.Srtm.SrtmDataCell"/> class.
		/// </summary>
		/// <param name='filepath'>
		/// Filepath.
		/// </param>
		/// <exception cref='FileNotFoundException'>
		/// Is thrown when a file path argument specifies a file that does not exist.
		/// </exception>
		/// <exception cref='ArgumentException'>
		/// Is thrown when an argument passed to a method is invalid.
		/// </exception>
		public SrtmDataCell (string filepath)
		{
			if (!File.Exists (filepath))
				throw new FileNotFoundException ("File not found.", filepath);
			
			if (string.Compare (".hgt", Path.GetExtension (filepath), StringComparison.CurrentCultureIgnoreCase) != 0)
				throw new ArgumentException ("Invalid extension.", filepath);
			
			string filename = Path.GetFileNameWithoutExtension (filepath).ToLower ();
			string[] fileCoordinate = filename.Split (new [] { 'e', 'w' });
			if (fileCoordinate.Length != 2)
				throw new ArgumentException ("Invalid filename.", filepath);
			
			fileCoordinate [0] = fileCoordinate [0].TrimStart (new [] { 'n', 's' });
			
			Latitude = int.Parse (fileCoordinate [0]);
			if (filename.Contains ("s"))
				Latitude *= -1;
			
			Longitude = int.Parse (fileCoordinate [1]);
			if (filename.Contains ("w"))
				Longitude *= -1;
			
			HgtData = File.ReadAllBytes (filepath);
			
			switch (HgtData.Length) {
			case 1201*1201*2: // SRTM-3
				PointsPerCell = 1201;
				break;
			case 3601*3601*2: // SRTM-1
				PointsPerCell = 3601;
				break;
			default:
				throw new ArgumentException ("Invalid file size.", filepath);
			}
		}
		
		#endregion
		
		#region Properties
		
		/// <summary>
		/// Gets or sets the hgt data.
		/// </summary>
		/// <value>
		/// The hgt data.
		/// </value>
		private byte[] HgtData { get; set; }
		
		/// <summary>
		/// Gets or sets the points per cell.
		/// </summary>
		/// <value>
		/// The points per cell.
		/// </value>
		private int PointsPerCell { get; set; }
		
		/// <summary>
		/// Gets or sets the latitude of the srtm data file.
		/// </summary>
		/// <value>
		/// The latitude.
		/// </value>
		public int Latitude { get; private set; }
		
		/// <summary>
		/// Gets or sets the longitude of the srtm data file.
		/// </summary>
		/// <value>
		/// The longitude.
		/// </value>
		public int Longitude { get; private set; }
		
		#endregion
		
		#region Public Methods
		
		/// <summary>
		/// Gets the elevation.
		/// </summary>
		/// <returns>
		/// The height. Null, if elevation is not available.
		/// </returns>
		/// <param name='coordinates'>
		/// Coordinates.
		/// </param>
		/// <exception cref='Exception'>
		/// Represents errors that occur during application execution.
		/// </exception>
		public int? GetElevation (IGeographicalCoordinates coordinates)
		{
            //int localLat = (int)((coordinates.Latitude - Latitude) * PointsPerCell);
			//int localLon = (int)(((coordinates.Longitude - Longitude)) * PointsPerCell);
            double localLat = ((coordinates.Latitude - Latitude) * PointsPerCell);
            double localLon = ((coordinates.Longitude - Longitude) * PointsPerCell);
            double xLat = localLat - Math.Truncate(localLat);
            double xLon = localLon - Math.Truncate(localLon);
            int intLocalLat = (int)localLat;
            int intLocalLon = (int)localLon;
            int? elevationlat0lon0 = GetElevationByXY(intLocalLat, intLocalLon);
            //return elevationlat0lon0;
            if (elevationlat0lon0 == null)
            {
                return null;
            }
            int? elevationlat1lon0 = GetElevationByXY(intLocalLat + 1, intLocalLon);
            int? elevationlat0lon1 = GetElevationByXY(intLocalLat, intLocalLon + 1);
            int? elevationlat1lon1 = GetElevationByXY(intLocalLat + 1, intLocalLon + 1);

            if (elevationlat1lon0 == null || elevationlat0lon1 == null || elevationlat1lon1 == null)
            {
                return elevationlat0lon0;
            }
            double result = ((double)elevationlat0lon0 * (1d - xLat) * (1d - xLon)) + ((double)elevationlat1lon0 * (xLat) * (1d - xLon)) + ((double)elevationlat0lon1 * (1d - xLat) * (xLon)) + ((double)elevationlat1lon1 * (xLat) * (xLon));
            return (int)result;
        }

        public int? GetElevationByXY(int localLat, int localLon)
        {
            int bytesPos = ((PointsPerCell - localLat - 1) * PointsPerCell * 2) + localLon * 2;

            if (bytesPos < 0 || bytesPos > PointsPerCell * PointsPerCell * 2)
                return 0;
                //throw new ArgumentOutOfRangeException("Coordinates out of range.", "coordinates");

            if ((HgtData[bytesPos] == 0x80) && (HgtData[bytesPos + 1] == 0x00))
                return null;

            // Motorola "big-endian" order with the most significant byte first
            return (HgtData[bytesPos]) << 8 | HgtData[bytesPos + 1];
        }
		
		#endregion
	}
}

