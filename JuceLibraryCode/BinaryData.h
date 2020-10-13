/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   crash_png;
    const int            crash_pngSize = 66857;

    extern const char*   default_bluxlayout;
    const int            default_bluxlayoutSize = 3611;

    extern const char*   icon_png;
    const int            icon_pngSize = 107960;

    extern const char*   icon2_png;
    const int            icon2_pngSize = 146775;

    extern const char*   icon3_png;
    const int            icon3_pngSize = 133862;

    extern const char*   icon128_png;
    const int            icon128_pngSize = 107960;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

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
