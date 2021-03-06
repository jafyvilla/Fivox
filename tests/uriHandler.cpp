
/* Copyright (c) 2015-2016, EPFL/Blue Brain Project
 *                          Jafet.VillafrancaDiaz@epfl.ch
 *
 * This file is part of Fivox <https://github.com/BlueBrain/Fivox>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of Eyescale Software GmbH nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#define BOOST_TEST_MODULE URIHandler

#include "test.h"
#include <fivox/uriHandler.h>
#include <brion/blueConfig.h>

#include <BBP/TestDatasets.h>

BOOST_AUTO_TEST_CASE(compartment_defaults)
{
    const fivox::URIHandler handler( "fivox://" );
    BOOST_CHECK_EQUAL( handler.getType(), fivox::VolumeType::TYPE_COMPARTMENTS );
    BOOST_CHECK_EQUAL( handler.getConfig().getCircuitSource(),
                 brion::BlueConfig( BBP_TEST_BLUECONFIG3 ).getCircuitSource( ));
    BOOST_CHECK_EQUAL( handler.getReport(), "voltages" );
    BOOST_CHECK_EQUAL( handler.getGIDs().size(), 50 );
    BOOST_CHECK_EQUAL( handler.getDt(), -1.f );
    BOOST_CHECK_EQUAL( handler.getDuration(), 10.0f );
    BOOST_CHECK_EQUAL( handler.getMaxBlockSize(), LB_64MB );
}

BOOST_AUTO_TEST_CASE(compartment_full_circuit)
{
    const fivox::URIHandler handler( "fivox://?target=*" );
    BOOST_CHECK_EQUAL( handler.getGIDs().size(), 1000 );
}

BOOST_AUTO_TEST_CASE(compartment_empty_target)
{
    BOOST_CHECK_THROW(
                fivox::URIHandler handler( "fivox://?target=EmptyTarget" ),
                std::runtime_error );
}

BOOST_AUTO_TEST_CASE(compartment_parameters)
{
    const fivox::URIHandler handler(
        "fivoxcompartment://?report=simulation&dt=0.2&target=Column" );
    BOOST_CHECK_EQUAL( handler.getGIDs().size(), 1000 );
    BOOST_CHECK_EQUAL( handler.getReport(), "simulation" );
    BOOST_CHECK_EQUAL( handler.getDt(), 0.2f );
}

BOOST_AUTO_TEST_CASE(compartment_targets)
{
    const fivox::URIHandler handler1(
        "fivoxcompartment://?target=mini50#Layer1" );
    BOOST_CHECK_EQUAL( handler1.getGIDs().size(), 50 );

    const fivox::URIHandler handler2(
        "fivoxcompartment://?report=simulation#Layer1" );
    BOOST_CHECK_EQUAL( handler2.getGIDs().size(), 20 );
    BOOST_CHECK_EQUAL( handler2.getReport(), "simulation" );
}

BOOST_AUTO_TEST_CASE(somas)
{
    const fivox::URIHandler handler( "fivoxsomas://" );
    BOOST_CHECK_EQUAL( handler.getGIDs().size(), 50 );
    BOOST_CHECK_EQUAL( handler.getType(), fivox::VolumeType::TYPE_SOMAS );
    BOOST_CHECK_EQUAL( handler.getReport(), "somas" );
}

BOOST_AUTO_TEST_CASE(spikes)
{
    const fivox::URIHandler handler( "fivoxspikes://" );
    BOOST_CHECK_EQUAL( handler.getType(), fivox::VolumeType::TYPE_SPIKES );
    BOOST_CHECK_EQUAL( handler.getReport(), "voltages" );
}

BOOST_AUTO_TEST_CASE(synapses)
{
    const fivox::URIHandler handler( "fivoxsynapses://" );
    BOOST_CHECK_EQUAL( handler.getType(), fivox::VolumeType::TYPE_SYNAPSES );
    BOOST_CHECK_EQUAL( handler.getReport(), "voltages" );
}

BOOST_AUTO_TEST_CASE(vsd)
{
    const fivox::URIHandler handler( "fivoxvsd://" );
    BOOST_CHECK_EQUAL( handler.getType(), fivox::VolumeType::TYPE_VSD );
    BOOST_CHECK_EQUAL( handler.getInputRange(),
                       fivox::Vector2f( -100000.f, 300.f ));
    BOOST_CHECK_EQUAL( handler.getDyeCurve(), "" );
    BOOST_CHECK_EQUAL( handler.getReport(), "voltages" );
}
