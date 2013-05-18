#ifndef SKYVO_UI_H
#define SKYVO_UI_H

#include <string>

namespace Common{

/**
* \class SkyvoUI
* \brief This class is an interface for a UI that the backend needs to use for all Skyvo Projects.
* Every GUI class must implement this interface.
* \author Seth Hendrick
*/
class SkyvoUI{
    public:

        /**
        * \enum YesNoDialogReturn
        * \brief the return value from a Yes/No/Cancel dialog.
        */
        enum YesNoDialogReturn{
            ABORT = 0, ///<Not cancel, as to not confuse with okay/cancel below
            NO,
            YES
        };

        /**
        * \enum OkayCancelDialogReturn
        * \brief the return value from an Okay/Cancel dialog.
        */
        enum OkayCancelDialogReturn{
            CANCEL = 0,
            OKAY
        };

        /**
        * \struct InputDialogReturn
        * \brief The return value
        */
        struct InputDialogReturn{
            std::string message;    ///<The message from the input box
            OkayCancelDialogReturn retValue; ///<Whether or not the user hit okay or cancel
        };

        virtual ~SkyvoUI(){}

        /**
        * \brief Produces a system notification.
        * This includes the win8 toaster notifications, or ios notifications
        * \param message The message to display
        */
        virtual void generateSystemNotification(std::string message) = 0;

        /**
        * \brief Produces a pop-up with the give message and an okay button
        * \param message The message to display
        */
        virtual void generateMessageBox(std::string message) = 0;

        /**
        * \brief Produces a pop-up error message with the given errorNumber, the message, and an okay button
        * \param errorNumber The error number
        * \param message The error message to display
        */
        virtual void generateErrorMessage(unsigned int errorNumber, std::string message) = 0;

        /**
        * \brief Contains a textbox and an okay/cancel button.
        * Hitting Cancel should not even look at whats in the textbox.  Hitting okay should return what ever is in the textbox, including the empty string.
        * \param message message to appear with Textbox
        * \return The message and OKAY if okay is pressed, else an empty string and CANCEL if cancel is hit
        */
        virtual InputDialogReturn generateInputDialog(std::string message) = 0;

        /**
        * \brief Produces a popup with a Yes, No, Cancel buttons, and a message.
        * Hitting Yes means the user wants to do the message, No means the user does not want to do the message, Cancel the user says "never mind"
        * \param message The message to display
        * \return YES if yes is pressed, NO if no is pressed, and ABORT if cancel is pressed.
        */
        virtual YesNoDialogReturn generateYesNoCancelDialog(std::string message) = 0;

        /**
        * \brief Produces a popup with a Okay and Cancel button, and a message.
        * Hitting Okay means the user wants to do the message, Cancel means they do not
        * \param message The message to display
        * \return OKAY if okay is pressed, CANCEL if cancel is pressed
        */
        virtual OkayCancelDialogReturn generateOkayCancelDialog(std::string message) = 0;
};

}

#endif
