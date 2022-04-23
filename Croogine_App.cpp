#include "Croogine_App.h"



namespace Croogine {

	void CroogineApp::run() {

        while (!croogineWindow.shouldClose()) //Check the close flag of the application window; if there is a click on the close button, it leaves the while()
        {
            glfwPollEvents(); //check all events (click, resize, close, move, etc.) and set flags accordingly
            //renderPipeline();
        }
	}

}