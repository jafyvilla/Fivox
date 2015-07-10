
/* Copyright (c) 2014-2015, EPFL/Blue Brain Project
 *                          Stefan.Eilemann@epfl.ch
 */

#define BOOST_TEST_MODULE CompartmentSource

#include "sdk.h"
#include <fivox/compartmentLoader.h>
#include <BBP/TestDatasets.h>

#include <itkTimeProbe.h>
#include <iomanip>

BOOST_AUTO_TEST_CASE( CompartmentSource )
{
    char** argv = boost::unit_test::framework::master_test_suite().argv;
    const bool unitTest = std::string( argv[0] ).find( "perf_" ) ==
                          std::string::npos;
    static const size_t maxSize = unitTest ? 8 : 512;
    const fivox::URIHandler params( "fivox://" );
    fivox::EventSourcePtr source =
        std::make_shared< fivox::CompartmentLoader >( params );
    source->load( 0.f );

#ifdef NDEBUG
    std::cout.setf( std::ios::right, std::ios::adjustfield );
    std::cout.precision( 5 );
    std::cout << "Comp Kernel, byte MVox/sec, float MVox/sec" << std::endl;
#endif

    for( size_t i = 1; i <= maxSize; i = i << 1 )
    {
        {
            itk::TimeProbe clock;
            clock.Start();
            _testSDKKernel< unsigned char >( i, source, 7 );
            clock.Stop();
#ifdef NDEBUG
            std::cout << std::setw( 11 ) << i << ',' << std::setw(14)
                      << i*i*i / 1024.f / 1024.f / clock.GetTotal();
#endif
        }
        if( true )
        {
            itk::TimeProbe clock;
            clock.Start();
            _testSDKKernel< float >( i, source, 0.0301621668f );
            clock.Stop();
#ifdef NDEBUG
            std::cout << ',' << std::setw(15)
                      << i*i*i / 1024.f / 1024.f / clock.GetTotal();
#endif
        }
#ifdef NDEBUG
        std::cout << std::endl;
#endif
    }
}
