#ifndef OPENMW_COMPONENTS_DETOURNAVIGATOR_NAVIGATORUTILS_H
#define OPENMW_COMPONENTS_DETOURNAVIGATOR_NAVIGATORUTILS_H

#include "findsmoothpath.hpp"
#include "flags.hpp"
#include "navigator.hpp"
#include "navmeshcacheitem.hpp"
#include "settings.hpp"

#include <components/misc/guarded.hpp>

#include <optional>

namespace DetourNavigator
{
    /**
     * @brief findPath fills output iterator with points of scene surfaces to be used for actor to walk through.
     * @param agentBounds allows to find navmesh for given actor.
     * @param start path from given point.
     * @param end path at given point.
     * @param includeFlags setup allowed surfaces for actor to walk.
     * @param out the beginning of the destination range.
     * @param endTolerance defines maximum allowed distance to end path point in addition to agentHalfExtents
     * @return Output iterator to the element in the destination range, one past the last element of found path.
     * Equal to out if no path is found.
     */
    template <class OutputIterator>
    inline Status findPath(const Navigator& navigator, const AgentBounds& agentBounds, const float stepSize,
        const osg::Vec3f& start, const osg::Vec3f& end, const Flags includeFlags, const AreaCosts& areaCosts,
        float endTolerance, OutputIterator out)
    {
        static_assert(std::is_same<typename std::iterator_traits<OutputIterator>::iterator_category,
                          std::output_iterator_tag>::value,
            "out is not an OutputIterator");
        const auto navMesh = navigator.getNavMesh(agentBounds);
        if (navMesh == nullptr)
            return Status::NavMeshNotFound;
        const Settings& settings = navigator.getSettings();
        auto outTransform = OutputTransformIterator<OutputIterator>(out, settings.mRecast);
        const auto locked = navMesh->lock();
        return findSmoothPath(locked->getImpl(), locked->getQuery(),
            toNavMeshCoordinates(settings.mRecast, agentBounds.mHalfExtents),
            toNavMeshCoordinates(settings.mRecast, stepSize), toNavMeshCoordinates(settings.mRecast, start),
            toNavMeshCoordinates(settings.mRecast, end), includeFlags, areaCosts, settings.mDetour, endTolerance,
            outTransform);
    }

    /**
     * @brief findRandomPointAroundCircle returns random location on navmesh within the reach of specified location.
     * @param agentBounds allows to find navmesh for given actor.
     * @param start is a position where the search starts.
     * @param maxRadius limit maximum distance from start.
     * @param includeFlags setup allowed surfaces for actor to walk.
     * @return not empty optional with position if point is found and empty optional if point is not found.
     */
    std::optional<osg::Vec3f> findRandomPointAroundCircle(const Navigator& navigator, const AgentBounds& agentBounds,
        const osg::Vec3f& start, const float maxRadius, const Flags includeFlags, float (*prng)());

    /**
     * @brief raycast finds farest navmesh point from start on a line from start to end that has path from start.
     * @param agentBounds allows to find navmesh for given actor.
     * @param start of the line
     * @param end of the line
     * @param includeFlags setup allowed surfaces for actor to walk.
     * @return not empty optional with position if point is found and empty optional if point is not found.
     */
    std::optional<osg::Vec3f> raycast(const Navigator& navigator, const AgentBounds& agentBounds,
        const osg::Vec3f& start, const osg::Vec3f& end, const Flags includeFlags);
}

#endif
