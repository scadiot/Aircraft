// See https://aka.ms/new-console-template for more information
using Common;
using HeightExtractor;

var heightFileGenerator = new HeightFileGenerator();

//heightFileGenerator.GenerateFile(17, 65312, 47208, 16, @"C:\TMP\aircraft\tiles\17");
//heightFileGenerator.loadData(42.811519, 0, 42.553083, 0.703129, @"C:\TMP\aircraft\tiles");
heightFileGenerator.LoadData(8, 128, 94, 16, @"C:\TMP\aircraft\tiles");
//Console.WriteLine("X: " + location.X + ", Y: " + location.Y);
Console.WriteLine("Hello, World!");
