///////////////////////////////////////////////////////////////////////////////
//
//  The contents of this file are subject to the Mozilla Public License
//  Version 1.1 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.mozilla.org/MPL/
//
//  Software distributed under the License is distributed on an "AS IS"
//  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
//  License for the specific language governing rights and limitations
//  under the License.
// 
//  The Original Code is MP4v2.
// 
//  The Initial Developer of the Original Code is Kona Blend.
//  Portions created by Kona Blend are Copyright (C) 2008.
//  All Rights Reserved.
//
//  Contributors:
//      Kona Blend, kona8lend@@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#include "impl.h"

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

class OptimizeUtility : public Utility
{
public:
    OptimizeUtility( int, char** );

protected:
    // delegates implementation
    bool utility_process();
    bool utility_job( JobContext& );

private:
    enum PseudoLong {
        PL_UNDEFINED = 0xf000000, // serves as safe starting value
        PL_VERBOSE,
        PL_HELP,
        PL_VERSION,
    };
};

///////////////////////////////////////////////////////////////////////////////

OptimizeUtility::OptimizeUtility( int argc, char** argv )
    : Utility( "mp4optimize", argc, argv )
{
    _usage = "[OPTION]... file...";
    _help =
        // 79-cols, inclusive, max desired width
        // |----------------------------------------------------------------------------|
        "\n"
        "\nFor each mp4 file specified, optimize the file structure by moving control"
        "\ninformation to the beginning of the file and interleaving track samples,"
        "\neliminating excess seeks during playback. Additionally free blocks and"
        "\nunreferenced data chunks are eliminated, reducing file size."
        "\n"
        "\nOPTIONS"
//      "\n  -y, --dryrun       do not actually create or modify any files"
        "\n  -k, --keepgoing    continue batch processing even after errors"
        "\n  -q, --quiet        equivalent to --verbose 0"
        "\n  -v, --verbose NUM  increase verbosity or long-option to set level NUM"
        "\n  -h, --help         print help or long-option for extended help"
        "\n      --version      output version information and exit";

    const prog::Option options[] = {
        { "dryrun",    prog::Option::NO_ARG,       0, 'y' },
        { "keepgoing", prog::Option::NO_ARG,       0, 'k' },
        { "quiet",     prog::Option::NO_ARG,       0, 'q' },
        { "verbose",   prog::Option::REQUIRED_ARG, 0, PL_VERBOSE },

        { "help",      prog::Option::NO_ARG,       0, PL_HELP },
        { "version",   prog::Option::NO_ARG,       0, PL_VERSION },

        { 0, prog::Option::NO_ARG, 0, 0 }
    };
    _options = options;
}

///////////////////////////////////////////////////////////////////////////////

bool
OptimizeUtility::utility_job( JobContext& job )
{
    verbose1f( "optimizing %s\n", job.file.c_str() );
    if( !MP4Optimize( job.file.c_str(), NULL ))
        return herrf( "optimize failed: %s\n", job.file.c_str() );

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

bool
OptimizeUtility::utility_process()
{
    for ( ;; ) {
        const int c = prog::getOption( _argc, _argv, "ykqvh", _options, NULL );
        if( c == -1 )
            break;

        switch ( c ) {
            case 'y':
                _dryrun = true;
                break;

            case 'k':
                _keepgoing = true;
                break;

            case 'q':
                _verbosity = 0;
                break;

            case 'v':
                _verbosity++;
                break;

            case 'h':
                printHelp( false );
                return SUCCESS;

            case PL_VERBOSE:
            {
                const uint32_t level = std::strtoul( prog::optarg, NULL, 0 );
                _verbosity = ( level < 4 ) ? level : 3;
                break;
            }

            case PL_HELP:
                printHelp( true );
                return SUCCESS;

            case PL_VERSION:
                printVersion();
                return SUCCESS;

            default:
                printUsage( true );
                return FAILURE;
        }
    }

    if( !(prog::optind < _argc) ) {
        printUsage( true );
        return FAILURE;
    }

    return batch( prog::optind );
}

///////////////////////////////////////////////////////////////////////////////

extern "C"
int main( int argc, char** argv )
{
    OptimizeUtility util( argc, argv );
    return util.process();
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
