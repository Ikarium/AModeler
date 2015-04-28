#pragma once

struct App
{
	static Model::ComponentsLibrary * componentsLibrary;
	static Model::TypesLibrary * typesLibrary;
	static Interface::TypesLibraryView * typesLibraryView;
	static Interface::ComponentsLibraryView * componentsLibraryView;
};