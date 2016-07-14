
/* Copyright (c) 2014-2016, EPFL/Blue Brain Project
 *                          Stefan.Eilemann@epfl.ch
 *
 * This file is part of Fivox <https://github.com/BlueBrain/Fivox>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FIVOX_IMAGESOURCE_HXX
#define FIVOX_IMAGESOURCE_HXX

#include "imageSource.h"

#include <fivox/densityFunctor.h>
#include <itkProgressReporter.h>
#include <itkImageRegionIteratorWithIndex.h>

namespace fivox
{
static const int _splitDirection = 2; // fastest in latest test

template< typename TImage > ImageSource< TImage >::ImageSource()
    : _functor( new DensityFunctor< TImage >( Vector2f( )))
    , _progressObserver( ProgressObserver::New( ))
{
//    itk::ImageRegionSplitterDirection::Pointer splitter =
//        itk::ImageRegionSplitterDirection::New();
//    splitter->SetDirection( _splitDirection );
//    _splitter = splitter;

    // set up default size
    static const size_t size = 256;
    typename TImage::SizeType vSize;
    vSize.Fill( size );
    typename TImage::RegionType region;
    region.SetSize( vSize );

    typename TImage::Pointer output = Superclass::GetOutput();
    output->SetRequestedRegion( region );

    Superclass::AddObserver( itk::ProgressEvent(), _progressObserver );
}

template< typename TImage >
typename ImageSource< TImage >::FunctorPtr ImageSource< TImage >::getFunctor()
{
    return _functor;
}

template< typename TImage >
void ImageSource< TImage >::setFunctor( FunctorPtr functor )
{
    _functor = functor;
}

template< typename TImage > void ImageSource< TImage >::showProgress()
{
    _progressObserver->enablePrint();
}

template< typename TImage >
void ImageSource< TImage >::PrintSelf(std::ostream & os, itk::Indent indent )
    const
{
    Superclass::PrintSelf( os, indent );
}

template< typename TImage >
void ImageSource< TImage >::GenerateData(
 /*   const ImageRegionType& outputRegionForThread, itk::ThreadIdType threadId */)
{
    _completed = 0;
    _functor->beforeGenerate();
    _progressObserver->reset();

    ImagePointer image = Superclass::GetOutput();
    typedef itk::ImageRegionIteratorWithIndex< TImage > ImageIterator;

    const typename TImage::SizeType vSize =
            image->GetLargestPossibleRegion().GetSize();
//    const size_t numPixels = vSize[0] * vSize[1] * vSize[2];

//    ImageIterator i( image, image->GetLargestPossibleRegion()/* outputRegionForThread */);
//    i.SetDirection(0);
//    i.GoToBegin();
//const itk::ThreadIdType threadId = 0;
//    const size_t nLines = this->GetOutput()->GetRequestedRegion().GetSize()[1] *
//                          this->GetOutput()->GetRequestedRegion().GetSize()[2];
//    itk::ProgressReporter progress( this, threadId, nLines );
//    size_t totalLines = 0;

    image->Allocate();

    const size_t ni = vSize[0];
    const size_t nj = vSize[1];
    const size_t nk = vSize[2];
    const typename TImage::SpacingType spacing = image->GetSpacing();
    #pragma omp parallel for schedule(auto)
    for( size_t i = 0; i < ni; ++i )
        for( size_t j = 0; j < nj; ++j )
            for( size_t k = 0; k < nk; ++k )
//    while( !i.IsAtEnd( ))
    {
//        const ImageIndexType& index = i.GetIndex();
        ImageIndexType index;
        index[0] = i;
        index[1] = j;
        index[2] = k;

        typename TImage::PointType point;
        image->TransformIndexToPhysicalPoint( index, point );

        image->SetPixel( index, (*_functor)( point, spacing ));
//        i.Set( (*_functor)( point, spacing ) );

//        ++i;
//        if( i.IsAtEndOfLine( ))
//        {
//            i.NextLine();
            // report progress only once per line for lower contention on
            // monitor. Main thread reports to itk, all others to the monitor.
//            if( threadId == 0 )
//            {
//                size_t done = _completed.set( 0 ) + 1 /*self*/;
//                totalLines += done;
//                while( done-- )
//                    progress.CompletedPixel();
//            }
//            else
//                ++_completed;
//        }
    }
////////////////////////// find out the omp thread id to update the progress bar
//    if( threadId == 0 )
//    {
//        while( totalLines < nLines )
//        {
//            _completed.waitNE( 0 );
//            size_t done = _completed.set( 0 );
//            totalLines += done;
//            while( done-- )
//                progress.CompletedPixel();
//        }
//    }
}

//template< typename TImage >
//void ImageSource< TImage >::BeforeThreadedGenerateData()
//{
//    _completed = 0;
//    _functor->beforeGenerate();
//    _progressObserver->reset();
//}

} // end namespace fivox

#endif
