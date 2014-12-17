#pragma once

#include "Interface/Components/SlotView.h"

#include "Model/Model/Class.h"

namespace Model
{

enum class SlotType{ input, output };

class Slot
{
private:

	Interface::SlotView * view_ = nullptr;
	SlotType slotType_;
	Type type_;
	uint pos_;
	Component* owner_;
	QString name_;
	List<Link*> links_;
	Link* uniqueLink_ = nullptr;

public:
	Slot(Component*, PropertyTree &);
	~Slot();

	/**************************
	PropertyTrees
	***************************/
	PropertyTree get() const;

	void set(PropertyTree &);

	void setLinks();

	/**************************
	Getter
	***************************/

	QString const & name() const;
	Type const & type() const;
	int const pos() const;
	Component const * owner() const;
	Component* owner();
	SlotType slotType();
	List<Link *> & links();
	Link * uniqueLink();
	Vector<Slot*> linkedSlots();
	Slot* linkedUniqueSlot();
	Interface::SlotView * view();
	Interface::SlotView * const view() const;

	/**************************
	Setter
	***************************/
	void pos(int pos);
	void setName(QString &);
	void setViewDeleted();

	bool canLink();

	void connectLink(Link *);
	void connectUniqueLink(Link *);
	void unlink(Link *);


	Slot & operator=(Slot const &);

	bool operator ==(Slot const &) const;

	bool operator !=(Slot const &) const; 

};

}