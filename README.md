# parlaycalc
# QT C++ Parlay Calc

This a little QC C++ programa crated just in order to learn.

This is a "parlay calculator".

(https://www.oddsshark.com/tools/parlay-calculator)

This type of calculator is used to determine the performance of a bet. This bet is supposed to combine different sporting events, each event has a coefficient. This coefficient increases the returned value of the bet. In principle, each success is a simple multiplication.

For example. Imagine, 2 events, the first hit pays 2.80, the second one 0.80. Putting a bet of 10.

 10.00 x First event  2.80  -> Payout 28.00  (win 18,00)
 28.00 x Second event 1.20  -> Payout 33,60  (win 23,60)


All bets must be right and each success increases your return. So each coefficient must be greater than 1. We need to validate this.


There are 3 different notation: Decimal, American, Fractional (for another project)


American notation assumes positive and negative values.

 +180 (american) y 2.8 decimal.  For this american value is divide by 100 + adding 1. 180/100 + 1.
 but, negative values like -120 are diferent  1 + 100/(American Value), so   1 - 100/(-120), da 1.833

+180 (American not.) es 2.8 decimal. The American value is divided by 100 + plus 1. 180/100 + 1.
  but, negative values such as -120 are calculated differently 1 + 100/(not. American), thus 1 - 100/(-120), gives 1.833


With the above we will have the opportunity to read numbers and validate range, decimals, etc.
We will need some arithmetic to convert between notations.

Between controls and logic we need to manage signals. And well, the controls will be two buttons (add, delete), two controls to read numbers that the user gives us. And since this type of bet has a list of sporting events, some list-type control that will display values as they are typed.



# Qt c++ structure



We have some files generated automatically, basically by the Qt designer, and others that are initially given to us after the project is created.

"main.cpp" which we didn't really touch and is the basic cpp to star.

"mainwindows.ui" (ui = user interface) which is an xml file with the layout of controls. As well as some properties.

Qt gives us a graphical editor to move and change controls/widget, properties, attributes and etc. and generates the result of this in the ui file. So we don't have to  do it ourselves, this is "Design mode".

"mainwindows.cpp" which along with "mainwindows.h" is where things get interesting. Here, in the MainWindow class, the elements that go inside our form are established.

As elements of the "MainWindow" class we are going to define some elements that we need (and are not or can be created via Qt's "Design Mode"). Among others, a routine to do parlay calculations, a pair of "validator" and an "ActionGroup".


The first thing that has been presented to us in the "design mode" is a form, with a "Type Here", a dotted area (grid that serves as a reference to the position of widgets) and a status bar below. The statusbar allows us to give messages that remain without getting in the way much. In our case we will have to notify the type of notation we are using.


Personally, I find menu management somewhat uncomfortable, and it is strange that you cannot click directly on the "status bar". To modify the properties of the status bar we have on the top right an Object inspector where we can search and select controls, in this case the status bar. Once the status bar is selected, however, there is not much that interests us.

We have defined a menu, at the top level "File" and "Mode". Under "File" "Exit" is defined (then a slot is defined to exit). Next under "mode" we have "Fractional", "American" and "Decimal".

# Status bar


It is the bar at the bottom of the form, we will only use it to display a text that will indicate the notation such as Decimal or American.

To do this we have two ways, we can use showMessade of that control, example:

    ui->statusbar->showMessage("American notation");

But when you hover, for example, the mouse over the menu is cleared. There is another solution.

The status bar can contain a Widget (here I usually refer to it as a control) like a "Label", something like this:

    ui->statusBar->addPermanentWidget(statusLabel);


Then we assign a text to that label instead of calling ShowMessage, we must therefore create and define this label in the MainWindows class. We define it below the private section, don't forget that we require #include <QLabel> at the beginning of the file. The definition is:


    QLabel *statusLabel;


In the private section of the MainWindows class we will place the controls (widgets) that we need to create by code.

When building the class we create our statusLabel widget and assign it to the statusbar.


    statusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(statusLabel);


# Signal/Slot, Auto-Connection


The mechanism for connecting a widget to an operation or code in C++ is Signal/Slot. For example we have a "Clear all" button that does a general cleaning.


## In "design mode"


To create everything necessary from "Design Mode" you must select the widget and right click on it (in this case the button that has the text "Clear all"). We must see the "Go to slot" option; by selecting it we will have access to different signals that this widget can generate. We are interested in "clicked()", selecting this a method of the MainWindows class is created (if it does not already exist), in fact in the slot section of the class, with the control name, signal name and a prefix "on_" , (on_btnClear_clicked). In this method we place the code of what we want to be done. In this case, clear of several elements in the form is executed.

The mechanism for a signal to trigger the slot requires a connection. With the method that has been used here (which I prefer) Qt is responsible for making that connection without us seeing it.

In this project the above method was used for several widgets. Not everything gives us the "Go to slot" option from the Qt designer. In this case we also have the Exit action from the menu.


## Signal and slot editor


The Qt designer shows a table where connections are established between signals and slots. This is 4 columns or elements that respectively are those that emit the signal, what the signal is, who receives it and in which slot.

In this project an entry was created like this. Under the "sender" column we are allowed to select between different elements, here we select "ActionExit" which was defined as an entry under the "File" menu (all this using the designer). This option does not have a click as such but rather "triggered" as a signal. Upon receiving this signal we want the form to close. Thus the receiver is MainWindow and it is sent to a slot called close() which precisely closes the form. Being the only form of this application and the main one, closing it terminates the application.

This table corresponds to the signal and slot connections. And in this case we have not had to write any code.

In addition to the Exit action we have those that define the notation to use. In this case we do not have a predefined slot that we want to activate. In fact we want to do several things:

        Change data entry validation. Decimal and American differ in range.
        Change the hint text in the edit box as appropriate.
        Change the label within the statusbar.

Surely we can handle some of this from the "Signal and slot editor", but validation is not defining a property and requires creating certain elements that are not at design time. On the other hand, we do not write to the status but to a widget that is inside it (a label inside the status bar) and does not exist at design time. So we move on to the "Action Editor".

## Action Editor


This element (view) of the designer shows us the defined actions (all those in the menu). We select ActionDecimal, right click "go to slot". We select "Triggered()" there and immediately end up in the cpp file where a method is defined as MainWindows slot.

In this code it is already possible to access the statusLabel (which is not defined at design time, but by code). Here we then do the different operations that the notation change affects (they will be explained later).

It should be noted that in this project "on_actionAmerican_triggered()" and "setdecimalvalidator()" have been created, one for the American mode and another for decimal. The first one from "action editor" and note that it starts with "on_" so it is automatically connected. The second was generated by code.

## by Code


"setdecimalvalidator" was created in the definition of the MainWindow class as slot. It is then defined as a method of the class in the MainWindon.cpp file. This has not been created by the Qt designer but is code written in a simple or direct way, and that is why Qt does not generate the signal/slot connection and that is why we include this statement in the constructor of the MainWindow class


        connect(ui->actionDecimal, SIGNAL(triggered()), this, SLOT(setdecimalvalidator()));

and voila...


## Auto-Connection


Autoconnection implies that the Qt framework takes the name of the methods defined in the slot section within the class and analyzes whether they correspond to the form:
        on_<sender_name>_<signal-name>

If applicable, it builds the connection automatically. The compiler generates a warning in this regard and it is interesting to give a reading on what it implies. You have to be careful not to change the name of a widget that has a signal/slot link because you can lose that connection with the potential of being a hard-to-detect bug.



# Edit widget and Validator


We have two data entry fields, this would be the amount to bet (a floating point number) and the coefficient (odd) of each bet (decimal or American mode, perhaps in a future fractional mode).

The widget to receive the wagered amount has been called "edwager", at the top of the form next to a label with the text "Total wager".

QT allows you to assign instances of the QValidator class to establish when a text is acceptable as data input. QT comes predefined among others QDoubleValidator that we built with the parameters that define the range (minimum and maximum) as well as the number of decimals.



We define betvalidator as QDoubleValidator and associate it with edwagger.

    betvalidator = new QDoubleValidator(0.0, 100000.0, 2); // with 2 decimals of precision
    ui->edwagger->setValidatorbetvalidator(dv);

Another validator is created for the coefficients, but slightly different. In this case the minimum value is 1

    decmodivalidator = new QDoubleValidator(1.0, 100000.0, 2); // Minimum 1, with 2 decimals of precision

decmodivalidator which is a QValidator is not assigned in the MainWindows build, it is assigned when decimal mode is set, and when American mode is set another validator is set. Basically we move the same field between two validators depending on the mode chosen by the user.

    amermodevalidator= new QDoubleValidator(); // No range, but decimals
    amermodevalidator->setDecimals(2);
    ui->edOdd->setValidator(amermodevalidator);


NOTE: betvalidator, decmodivalidator have not been associated with a "parent" (although it is possible). Since they have no parent, their elimination must be encoded. In the destructor of the MainVindows class, the corresponding delete is done, which frees the use of memory and resources.

We have a button that processes each odds, copying it to a list and calculating the total to win. That button named btnAdd has an associated slot (from the designer) that does what is required.

In addition, the coefficient editing wiget (edOdd) is triggering a signal to the on_edOdd_returnPressed() slot, and this in turn calls the slot associated with the button.

The above is not good practice (I seem to remember it being an anti-design pattern). We have several options:


1. Each widget generates a signal and is associated with a slot/method that performs the operation (calculate and add to list, this is repetitive).

2. Each widget generates a signal and **points to the same slot**. It is interesting and perfectly valid, but it has to be done by code, the QT designer, in the form we have used, does not make that easy for us, using the "Edit Slot/Signal" design mode if it should be possible, just not It is part of the objective of this project.

3. We have two signals from two different sources, which go to **two different slots**, one slot calls the other. This is how it is currently being done in the project. As I wanted to demonstrate that a slot/method can be called directly, this is the path taken, however the next option is best.

4. Create a private class routine that does the calculation and adds the value to the list. Each slot makes the call to this private routine.




# QActionGroup


Each "action" associated with a menu can function as a selection box (Checkbox), but we are more interested in it as a **radio button** (the old analogue vehicle radios where a station was selected by pressing a button and the button on the previous selection emerged). This is achieved in QT by defining a group within which are the "Actions" that are mutually exclusive (If Decimal is selected, any other mode is automatically deselected). We have QActionGroup for this.

    alignmentGroup = new QActionGroup(this);

    // after the group is defined, the actions part of it are associated.

    alignmentGroup->addAction(ui->actionAmerican);
    alignmentGroup->addAction(ui->actionDecimal);
    alignmentGroup->addAction(ui->actionFractional);


With this we will see that the menu that previously had a square with a Check symbol is now a circle.


# listWidget


Each coefficient (odd) that we process is noted in a list, (listwidget). The reason for this in principle is almost aesthetic, leaving the user with a history. But the idea was to eventually be able to remove some item from the list with a right click. It remains something to do.

In any case with this widget the only thing we do is add lines, this is **addItem**.

    ui->listWidget->addItem( <string> );


# menu (action)


The menu is built in the **QT designer**, in particular each submenu has been generated as a separate action; for output and to select mode, nothing more.

