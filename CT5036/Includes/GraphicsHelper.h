//////////////////////////////
// File: GraphicsHelper.h.
// Author: Liam Bansal.
// Date Created: 15/11/2020.
//////////////////////////////

/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#ifndef GRAPHICS_HELPER_H
#define GRAPHICS_HELPER_H

class GraphicsHelper
{
public:
    void Initialize();
    void Finalize();
    void SwapBuffers();
};

#endif // GRAPHICS_HELPER_H.
