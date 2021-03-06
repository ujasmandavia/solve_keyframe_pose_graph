#pragma once


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <ostream>
#include <cstdlib>

//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
using namespace Eigen;
#include <opencv2/core/eigen.hpp>

using namespace std;

#define WITH_NLOHMANN_JSON
#ifdef WITH_NLOHMANN_JSON
#include "../nlohmann/json.hpp"
using json = nlohmann::json;

#endif

#define __RawFileIO__write_image_debug_dm( msg ) msg;

#include <sys/stat.h> // for is_directory


class RawFileIO
{
public:
    static void write_image( string fname, const cv::Mat& img);
    static void write_string( string fname, const string& my_string);

    // templated static function canot only exist in header files.
    template <typename Derived>
    static void write_EigenMatrix(const string& filename, const MatrixBase<Derived>& a)
    {
      // string base = string("/home/mpkuse/Desktop/bundle_adj/dump/datamgr_mateigen_");
      std::ofstream file(filename);
      if( file.is_open() )
      {
        // file << a.format(CSVFormat) << endl;
        file << a << endl;
        __RawFileIO__write_image_debug_dm(std::cout << "\033[1;32m" <<"write_EigenMatrix: "<< filename  << "    size=" << a.rows() << "x" << a.cols() << "\033[0m\n";)
      }
      else
      {
        cout << "\033[1;31m" << "FAIL TO OPEN FILE for writing: "<< filename << "\033[0m\n";

      }
    }


    static void write_Matrix2d( const string& filename, const double * D, int nRows, int nCols );
    static void write_Matrix1d( const string& filename, const double * D, int n  );

    static bool read_eigen_matrix( string filename, MatrixXd& result );
    static bool read_eigen_matrix( string filename, Matrix4d& result );
    static bool read_eigen_matrix( string filename, Matrix3d& result );
    static bool read_eigen_matrix( string filename, VectorXi& result );

    ///< read the flat vector ary as a rowmajor matrix.
    /// [ 1, 2, 3, 4,5,6...,16 ] ==> [ [1,2,3,4], [5,6,7,8], [9,10,11,12], [13,14,15,16] ]
    /// TODO: Have a flag to read interpret the 1d array as a colmajor.
    static bool read_eigen_matrix( const std::vector<double>& ary, Matrix4d& result );

    #ifdef WITH_NLOHMANN_JSON
    // The input json need to be something like:
    // A new row is denoted with \n, and a comma separates elements.
    //{
    //            "cols": 4,
    //            "rows": 4,
    //            "data": "0.2857131543876468, -0.2530077727001951, 0.9243132912401226, -0.02953755668229465\n-0.9566719337894203, -0.01884313243445668, 0.2905576491157474, 0.2114882406102183,\n-0.05609638588601445, -0.9672807262182707, -0.2474291659792429, 0.04534835279466286\n0, 0, 0, 1"
    // }
    static bool read_eigen_matrix_fromjson( const json str, MatrixXd&  output );
    static bool read_eigen_matrix4d_fromjson( const json str, Matrix4d&  output );
    static bool read_eigen_vector_fromjson( const json str, VectorXd&  output );

    template <typename Derived>
    static json eigen_matrix_to_json( const MatrixBase<Derived>& a )
    {
        IOFormat CSVFormat(FullPrecision, DontAlignCols, ", ", "\n");
        json obj;
        obj["rows"] = a.rows();
        obj["cols"] = a.cols();
        std::stringstream ss;
        ss <<  a.format(CSVFormat);
        obj["data"] = ss.str();

        return obj;
    }
    #endif

    static bool if_file_exist( char * fname );
    static bool if_file_exist( string fname );

    static bool is_path_a_directory(const char* path);
    static bool is_path_a_directory(const string path);

    static int exec_cmd( const string& cmd ); //< Executes a unix command.


    static std::vector<std::string>
    split( std::string const& original, char separator );
};
