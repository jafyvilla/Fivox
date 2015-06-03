/* Copyright (c) 2014-2015, EPFL/Blue Brain Project
 *                          Stefan.Eilemann@epfl.ch
 *                          Jafet.VillafrancaDiaz@epfl.ch
 */

#ifndef FIVOX_COMPARTMENTLOADER_H
#define FIVOX_COMPARTMENTLOADER_H

#include <fivox/eventSource.h> // base class

namespace fivox
{
/** Loads compartment report data to be sampled by an EventFunctor. */
class CompartmentLoader : public EventSource
{
public:
    /**
    * Construct a new compartment event source.
    *
    * @param blueconfig The Blueconfig file for the simulation
    * @param target The target to load
    * @param report The name of the report to use, 'voltage' if empty
    * @param time The initial frame (time step)
    * @throw H5::exception or std::exception on error
    */
    CompartmentLoader( const std::string& blueconfig,
                       const std::string& target, const std::string& report,
                       float dt );

    virtual ~CompartmentLoader(); //!< Destruct this compartment event source

    /**
    * Load a new frame, based on the duration defined for each time step (dt)
    * @param frame The frame number to be loaded
    */
    void load( uint32_t frame );

private:
    class Impl;
    std::unique_ptr< Impl > _impl;
};
}

#endif
