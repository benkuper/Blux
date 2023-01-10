/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   multipoint_png;
    const int            multipoint_pngSize = 3119;

    extern const char*   noise_png;
    const int            noise_pngSize = 1839;

    extern const char*   point_png;
    const int            point_pngSize = 2756;

    extern const char*   rainbow_png;
    const int            rainbow_pngSize = 4481;

    extern const char*   solidcolor_png;
    const int            solidcolor_pngSize = 4514;

    extern const char*   strobe_png;
    const int            strobe_pngSize = 5621;

    extern const char*   node_png;
    const int            node_pngSize = 3682;

    extern const char*   picture_png;
    const int            picture_pngSize = 3156;

    extern const char*   script_png;
    const int            script_pngSize = 2116;

    extern const char*   timeline_png;
    const int            timeline_pngSize = 1253;

    extern const char*   video_png;
    const int            video_pngSize = 6004;

    extern const char*   crash_png;
    const int            crash_pngSize = 66857;

    extern const char*   default_bluxlayout;
    const int            default_bluxlayoutSize = 5554;

    extern const char*   fx_png;
    const int            fx_pngSize = 2635;

    extern const char*   icon_png;
    const int            icon_pngSize = 107960;

    extern const char*   icon2_png;
    const int            icon2_pngSize = 146775;

    extern const char*   icon3_png;
    const int            icon3_pngSize = 133862;

    extern const char*   icon128_png;
    const int            icon128_pngSize = 107960;

    extern const char*   link_png;
    const int            link_pngSize = 1731;

    extern const char*   seq_png;
    const int            seq_pngSize = 1994;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 20;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
