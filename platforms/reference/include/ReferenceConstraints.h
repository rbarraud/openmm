#ifndef OPENMM_REFERENCECONSTRAINTS_H_
#define OPENMM_REFERENCECONSTRAINTS_H_

/* -------------------------------------------------------------------------- *
 *                                   OpenMM                                   *
 * -------------------------------------------------------------------------- *
 * This is part of the OpenMM molecular simulation toolkit originating from   *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2013 Stanford University and the Authors.           *
 * Authors: Peter Eastman                                                     *
 * Contributors:                                                              *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR      *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 * USE OR OTHER DEALINGS IN THE SOFTWARE.                                     *
 * -------------------------------------------------------------------------- */

#include "ReferenceConstraintAlgorithm.h"
#include "ReferenceCCMAAlgorithm.h"
#include "ReferenceSETTLEAlgorithm.h"
#include "openmm/System.h"

namespace OpenMM {

/**
 * This class uses multiple algorithms to apply constraints as efficiently as possible.  It identifies clusters
 * of three atoms that can be handled by SETTLE, and creates a ReferenceSETTLEAlgorithm object to handle them.
 * It then creates a ReferenceCCMAAlgorithm object to handle any remaining constraints.
 */
class OPENMM_EXPORT ReferenceConstraints : public ReferenceConstraintAlgorithm {
public:
    ReferenceConstraints(const System& system, RealOpenMM tolerance);
    ~ReferenceConstraints();
    /**
     * Get the constraint tolerance.
     */
    RealOpenMM getTolerance() const;

    /**
     * Set the constraint tolerance.
     */
    void setTolerance(RealOpenMM tolerance);

    /**
     * Apply the constraint algorithm.
     * 
     * @param numberOfAtoms    number of atoms
     * @param atomCoordinates  the original atom coordinates
     * @param atomCoordinatesP the new atom coordinates
     * @param inverseMasses    1/mass
     */
    int apply(int numberOfAtoms, std::vector<OpenMM::RealVec>& atomCoordinates, std::vector<OpenMM::RealVec>& atomCoordinatesP, std::vector<RealOpenMM>& inverseMasses);

    /**
     * Apply the constraint algorithm to velocities.
     * 
     * @param numberOfAtoms    number of atoms
     * @param atomCoordinates  the atom coordinates
     * @param atomCoordinatesP the velocities to modify
     * @param inverseMasses    1/mass
     */
    int applyToVelocities(int numberOfAtoms, std::vector<OpenMM::RealVec>& atomCoordinates, std::vector<OpenMM::RealVec>& velocities, std::vector<RealOpenMM>& inverseMasses);
private:
    RealOpenMM tolerance;
    ReferenceCCMAAlgorithm* ccma;
    ReferenceSETTLEAlgorithm* settle;
};

} // namespace OpenMM

#endif /*OPENMM_REFERENCECONSTRAINTS_H_*/
