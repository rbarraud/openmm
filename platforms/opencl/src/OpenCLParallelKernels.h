#ifndef OPENMM_OPENCLPARALLELKERNELS_H_
#define OPENMM_OPENCLPARALLELKERNELS_H_

/* -------------------------------------------------------------------------- *
 *                                   OpenMM                                   *
 * -------------------------------------------------------------------------- *
 * This is part of the OpenMM molecular simulation toolkit originating from   *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2011 Stanford University and the Authors.           *
 * Authors: Peter Eastman                                                     *
 * Contributors:                                                              *
 *                                                                            *
 * This program is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
 * -------------------------------------------------------------------------- */

#include "OpenCLPlatform.h"
#include "OpenCLContext.h"
#include "OpenCLKernels.h"

namespace OpenMM {

/**
 * This kernel is invoked at the beginning and end of force and energy computations.  It gives the
 * Platform a chance to clear buffers and do other initialization at the beginning, and to do any
 * necessary work at the end to determine the final results.
 */
class OpenCLParallelCalcForcesAndEnergyKernel : public CalcForcesAndEnergyKernel {
public:
    OpenCLParallelCalcForcesAndEnergyKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data);
    OpenCLCalcForcesAndEnergyKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcForcesAndEnergyKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     */
    void initialize(const System& system);
    /**
     * This is called at the beginning of each force/energy computation, before calcForcesAndEnergy() has been called on
     * any ForceImpl.
     *
     * @param context       the context in which to execute this kernel
     * @param includeForce  true if forces should be computed
     * @param includeEnergy true if potential energy should be computed
     */
    void beginComputation(ContextImpl& context, bool includeForce, bool includeEnergy);
    /**
     * This is called at the end of each force/energy computation, after calcForcesAndEnergy() has been called on
     * every ForceImpl.
     *
     * @param context       the context in which to execute this kernel
     * @param includeForce  true if forces should be computed
     * @param includeEnergy true if potential energy should be computed
     * @return the potential energy of the system.  This value is added to all values returned by ForceImpls'
     * calcForcesAndEnergy() methods.  That is, each force kernel may <i>either</i> return its contribution to the
     * energy directly, <i>or</i> add it to an internal buffer so that it will be included here.
     */
    double finishComputation(ContextImpl& context, bool includeForce, bool includeEnergy);
private:
    class BeginComputationTask;
    class FinishComputationTask;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
    std::vector<long long> completionTimes;
    std::vector<int> contextTiles;
};

/**
 * This kernel is invoked by HarmonicBondForce to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLParallelCalcHarmonicBondForceKernel : public CalcHarmonicBondForceKernel {
public:
    OpenCLParallelCalcHarmonicBondForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcHarmonicBondForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcHarmonicBondForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the HarmonicBondForce this kernel will be used for
     */
    void initialize(const System& system, const HarmonicBondForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by CustomBondForce to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLParallelCalcCustomBondForceKernel : public CalcCustomBondForceKernel {
public:
    OpenCLParallelCalcCustomBondForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcCustomBondForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcCustomBondForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the CustomBondForce this kernel will be used for
     */
    void initialize(const System& system, const CustomBondForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by HarmonicAngleForce to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLParallelCalcHarmonicAngleForceKernel : public CalcHarmonicAngleForceKernel {
public:
    OpenCLParallelCalcHarmonicAngleForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcHarmonicAngleForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcHarmonicAngleForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the HarmonicAngleForce this kernel will be used for
     */
    void initialize(const System& system, const HarmonicAngleForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by CustomAngleForce to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLParallelCalcCustomAngleForceKernel : public CalcCustomAngleForceKernel {
public:
    OpenCLParallelCalcCustomAngleForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcCustomAngleForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcCustomAngleForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the CustomAngleForce this kernel will be used for
     */
    void initialize(const System& system, const CustomAngleForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by PeriodicTorsionForce to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLParallelCalcPeriodicTorsionForceKernel : public CalcPeriodicTorsionForceKernel {
public:
    OpenCLParallelCalcPeriodicTorsionForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcPeriodicTorsionForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcPeriodicTorsionForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the PeriodicTorsionForce this kernel will be used for
     */
    void initialize(const System& system, const PeriodicTorsionForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
    class Task;
private:
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by RBTorsionForce to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLParallelCalcRBTorsionForceKernel : public CalcRBTorsionForceKernel {
public:
    OpenCLParallelCalcRBTorsionForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcRBTorsionForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcRBTorsionForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the RBTorsionForce this kernel will be used for
     */
    void initialize(const System& system, const RBTorsionForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by CMAPTorsionForce to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLParallelCalcCMAPTorsionForceKernel : public CalcCMAPTorsionForceKernel {
public:
    OpenCLParallelCalcCMAPTorsionForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcCMAPTorsionForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcCMAPTorsionForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the CMAPTorsionForce this kernel will be used for
     */
    void initialize(const System& system, const CMAPTorsionForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by CustomTorsionForce to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLParallelCalcCustomTorsionForceKernel : public CalcCustomTorsionForceKernel {
public:
    OpenCLParallelCalcCustomTorsionForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcCustomTorsionForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcCustomTorsionForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the CustomTorsionForce this kernel will be used for
     */
    void initialize(const System& system, const CustomTorsionForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by NonbondedForce to calculate the forces acting on the system.
 */
class OpenCLParallelCalcNonbondedForceKernel : public CalcNonbondedForceKernel {
public:
    OpenCLParallelCalcNonbondedForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcNonbondedForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcNonbondedForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the NonbondedForce this kernel will be used for
     */
    void initialize(const System& system, const NonbondedForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by CustomNonbondedForce to calculate the forces acting on the system.
 */
class OpenCLParallelCalcCustomNonbondedForceKernel : public CalcCustomNonbondedForceKernel {
public:
    OpenCLParallelCalcCustomNonbondedForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcCustomNonbondedForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcCustomNonbondedForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the CustomNonbondedForce this kernel will be used for
     */
    void initialize(const System& system, const CustomNonbondedForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by CustomExternalForce to calculate the forces acting on the system and the energy of the system.
 */
class OpenCLParallelCalcCustomExternalForceKernel : public CalcCustomExternalForceKernel {
public:
    OpenCLParallelCalcCustomExternalForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcCustomExternalForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcCustomExternalForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the CustomExternalForce this kernel will be used for
     */
    void initialize(const System& system, const CustomExternalForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

/**
 * This kernel is invoked by CustomHbondForce to calculate the forces acting on the system.
 */
class OpenCLParallelCalcCustomHbondForceKernel : public CalcCustomHbondForceKernel {
public:
    OpenCLParallelCalcCustomHbondForceKernel(std::string name, const Platform& platform, OpenCLPlatform::PlatformData& data, System& system);
    OpenCLCalcCustomHbondForceKernel& getKernel(int index) {
        return dynamic_cast<OpenCLCalcCustomHbondForceKernel&>(kernels[index].getImpl());
    }
    /**
     * Initialize the kernel.
     *
     * @param system     the System this kernel will be applied to
     * @param force      the CustomHbondForce this kernel will be used for
     */
    void initialize(const System& system, const CustomHbondForce& force);
    /**
     * Execute the kernel to calculate the forces and/or energy.
     *
     * @param context        the context in which to execute this kernel
     * @param includeForces  true if forces should be calculated
     * @param includeEnergy  true if the energy should be calculated
     * @return the potential energy due to the force
     */
    double execute(ContextImpl& context, bool includeForces, bool includeEnergy);
private:
    class Task;
    OpenCLPlatform::PlatformData& data;
    std::vector<Kernel> kernels;
};

} // namespace OpenMM

#endif /*OPENMM_OPENCLPARALLELKERNELS_H_*/
