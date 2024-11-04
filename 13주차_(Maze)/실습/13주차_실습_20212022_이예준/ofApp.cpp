#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

	hPopup = menu->AddPopupMenu(hMenu, "DFS");
	
	hPopup = menu->AddPopupMenu(hMenu, "BFS");

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		if (isOpen) {
			freeMemory();
		}
		readFile();
	}
	if(title == "Exit") {
		if (isOpen) {
			freeMemory();
		}
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}



} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	ofSetRectMode(OF_RECTMODE_CENTER);
	for (i = 0; i < HEIGHT; i++){
		for (j = 0; j < WIDTH; j++) {
            int x1 = 5 + (j * 30);
            int y1 = 5 + (i * 30);
            int x2 = 5 + ((j + 1) * 30);
            int y2 = 5 + ((i + 1) * 30);
    
            ofDrawRectangle(x1, y1, 5, 5);
            ofDrawRectangle(x1, y2, 5, 5);
            ofDrawRectangle(x2, y1, 5, 5);
            ofDrawRectangle(x2, y2, 5, 5);

            if (Maze[i][j]->up == NULL) ofDrawLine(x1, y1, x2, y1);
            if (Maze[i][j]->down == NULL) ofDrawLine(x1, y2, x2, y2);
            if (Maze[i][j]->left == NULL) ofDrawLine(x1, y1, x1, y2);
            if (Maze[i][j]->right == NULL) ofDrawLine(x2, y1, x2, y2);
		}
	}


	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			if (isOpen) {
				freeMemory();
			}
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName.c_str());
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;
			
			
			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
            
            vector<string> lines;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				lines.push_back(line);
				cnt++;
			}

			HEIGHT = cnt / 2;
			WIDTH = lines[0].length() / 2;

			input = (char**)malloc(sizeof(char*) * cnt);
			for (int i = 0; i < cnt; i++) {
				input[i] = (char*)malloc(sizeof(char) * (lines[i].length() + 1));
				strcpy(input[i], lines[i].c_str());
			}

			Maze = (maze***)malloc(sizeof(maze**) * HEIGHT);
			for (int i = 0; i < HEIGHT; i++) {
				Maze[i] = (maze**)malloc(sizeof(maze*) * WIDTH);
				for (int j = 0; j < WIDTH; j++) {
					Maze[i][j] = (maze*)malloc(sizeof(maze));
				}
			}

			int i, j;
			for (i = 0; i < HEIGHT; i++) {
				for (j = 0; j < WIDTH; j++) {
                    
					if (input[i * 2][(j * 2) + 1] == ' ') {
						Maze[i][j]->up = Maze[i - 1][j];
						if (i) Maze[i - 1][j]->down = Maze[i][j];
					} else {
						Maze[i][j]->up = NULL;
						if (i) Maze[i - 1][j]->down = NULL;
					}
                    
					if (input[(i + 1) * 2][(j * 2) + 1] == ' ') {
						Maze[i][j]->down = Maze[i + 1][j];
						if (i != HEIGHT - 1) Maze[i + 1][j]->up = Maze[i][j];
					} else {
						Maze[i][j]->down = NULL;
						if (i != HEIGHT - 1) Maze[i + 1][j]->up = NULL;
					}
                    
					if (input[(i * 2) + 1][(j * 2)] == ' ') {
						Maze[i][j]->left = Maze[i][j - 1];
						if (j) Maze[i][j - 1]->right = Maze[i][j];
					} else {
						Maze[i][j]->left = NULL;
						if (j) Maze[i][j - 1]->right = NULL;
					}
                    
					if (input[(i * 2) + 1][(j + 1) * 2] == ' ') {
						Maze[i][j]->right = Maze[i][j + 1];
						if (j != WIDTH - 1) Maze[i][j + 1]->left = Maze[i][j];
					} else {
						Maze[i][j]->right = NULL;
						if (j != WIDTH - 1) Maze[i][j + 1]->left = NULL;
					}
                    
                    Maze[i][j]->visit = false;
                    Maze[i][j]->revisit = false;
				}
			}

			root = Maze[0][0];
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {
	int i;
	for (i = 0; i < (HEIGHT * 2) + 1; i++) {
		free(input[i]);
	}
	free(input);
	for (i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			free(Maze[i][j]);
		}
		free(Maze[i]);
	}
	free(Maze);
}

bool ofApp::DFS()
{
    maze * ExitMaze = Maze[HEIGHT - 1][WIDTH - 1];
    maze * curidx = root;
    stack<maze*> MazeStack;
    
    MazeStack.push(curidx);
    curidx->visit = true;

    while (!MazeStack.empty()) {
        curidx = MazeStack.top();

        if (curidx == ExitMaze) break;
        
        if (curidx->up != NULL && !curidx->up->visit) {
            curidx->up->visit = true;
            MazeStack.push(curidx->up);
        } else if (curidx->down != NULL && !curidx->down->visit) {
            curidx->down->visit = true;
            MazeStack.push(curidx->down);
        } else if (curidx->right != NULL && !curidx->right->visit) {
            curidx->right->visit = true;
            MazeStack.push(curidx->right);
        } else if (curidx->left != NULL && !curidx->left->visit) {
            curidx->left->visit = true;
            MazeStack.push(curidx->left);
		} else {
			curidx->revisit = true;
			MazeStack.pop();
		}
    }

    isDFS = 1;
    isdfs = 1;

	return true;
}
void ofApp::dfsdraw()
{
    ofSetColor(0, 255, 0);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (Maze[i][j]->visit) {
                int x = 20 + (j * 30);
                int y = 20 + (i * 30);

                if (Maze[i][j]->left && Maze[i][j]->left->visit) {
                        ofDrawLine(x, y, x - 30, y);
                }
                if (Maze[i][j]->down && Maze[i][j]->down->visit) {
                    ofDrawLine(x, y, x, y + 30);
                }
                if (Maze[i][j]->right && Maze[i][j]->right->visit) {
                    ofDrawLine(x, y, x + 30, y);
                }
                if (Maze[i][j]->up && Maze[i][j]->up->visit) {
                    ofDrawLine(x, y, x, y - 30);
                }
            }
        }
    }


    ofSetColor(192, 192, 192);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (Maze[i][j]->visit) {
                int x = 20 + (j * 30);
                int y = 20 + (i * 30);

                if (Maze[i][j]->left && Maze[i][j]->left->revisit) {
                    ofDrawLine(x, y, x - 30, y);
                }
                if (Maze[i][j]->down && Maze[i][j]->down->revisit) {
                    ofDrawLine(x, y, x, y + 30);
                }
                if (Maze[i][j]->right && Maze[i][j]->right->revisit) {
                    ofDrawLine(x, y, x + 30, y);
                }
                if (Maze[i][j]->up && Maze[i][j]->up->revisit) {
                    ofDrawLine(x, y, x, y - 30);
                }
            }
        }
    }
}

