#include "androidinterface.h"
#include <jni.h>
#include <QtAndroidExtras/QAndroidJniEnvironment>

androidInterface::androidInterface(QObject *parent) : QObject(parent)
{

}

QString androidInterface::getExternalStorageDirectory()
{
    QAndroidJniEnvironment qjniEnv;
    if (qjniEnv.javaVM())
    {
        JNIEnv *currEnv;
        if (qjniEnv.javaVM()->AttachCurrentThread((JNIEnv**)&currEnv, NULL)<0)
        {
            return "Error: cannot connect to JNI environment";
        }

        //Creates an Android Environment class
        const char * envClassName;
        envClassName = "android/os/Environment"; //Class Name
        jclass envClass; //JNI Environment class
        envClass = currEnv->FindClass(envClassName); //Finds the class
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot find class android/os/Environment";
        }
        if (envClass == NULL)
        {
            return "Error: Androind environment class is NULL";
        }

        //Call the methods getExternalStorageDirectory() in the Android Environment class
        jmethodID midExternalStorageDirectory; //JNI MethodID;
        midExternalStorageDirectory = currEnv->GetStaticMethodID(envClass,"getExternalStorageDirectory","()Ljava/io/File;"); //Gets the Method ID
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot get method ID for getExternalStorageDirectory";
        }

        jobject file; //The file object that return getExternalStorageDirectory
        file = currEnv->CallStaticObjectMethod(envClass,midExternalStorageDirectory); //Calls getExternalStorageDirectory
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot execute method getExternalStorageDirectory";
        }

        //Created a Java File Class
        const char * fileClassName;
        fileClassName = "java/io/File"; //Class Name
        jclass fileClass; //File class

        //Gets the class name
        fileClass = currEnv->FindClass(fileClassName); //Finds the class
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot find class java/io/File";
        }
        if (fileClass == NULL)
        {
            return "Error: Java File class is NULL";
        }

        //Call the methhods getAbsolutePath in the Java File class
        jmethodID midGetAbsolutePath; //JNI Method ID
        midGetAbsolutePath = currEnv->GetMethodID(fileClass,"getAbsolutePath","()Ljava/lang/String;"); //Gets the method ID
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot get method ID for getAbsolutePath";
        }

        jstring path; //String path
        path = (jstring)currEnv->CallObjectMethod(file,midGetAbsolutePath); //Calls getAbsolutePath passing the file JObject
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot execute method getAbsolutePath";
        }

        //Converts the JString into char*
        const char * ppath;
        ppath = currEnv->GetStringUTFChars(path, 0);

        //Finally create the return QString variable using the char*
        QString ExternalStorageDirectory(ppath);

        //Detach the urrent thread and free memory
        qjniEnv.javaVM()->DetachCurrentThread();

        //Return the path
        return ExternalStorageDirectory;
    }
    else
    {
        return "Error Java VM is empty";
    }
}

QString androidInterface::getDataDirectory()
{
    QAndroidJniEnvironment qjniEnv;
    if (qjniEnv.javaVM())
    {
        JNIEnv *currEnv;
        if (qjniEnv.javaVM()->AttachCurrentThread((JNIEnv**)&currEnv, NULL)<0)
        {
            return "Error: cannot connect to JNI environment";
        }

        //Creates an Android Environment class
        const char * envClassName;
        envClassName = "android/os/Environment"; //Class Name
        jclass envClass; //JNI Environment class
        envClass = currEnv->FindClass(envClassName); //Finds the class
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot find class android/os/Environment";
        }
        if (envClass == NULL)
        {
            return "Error: Androind environment class is NULL";
        }

        //Call the methods getExternalStorageDirectory() in the Android Environment class
        jmethodID midDataDirectory; //JNI MethodID;
        midDataDirectory = currEnv->GetStaticMethodID(envClass,"getDataDirectory","()Ljava/io/File;"); //Gets the Method ID
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot get method ID for getExternalStorageDirectory";
        }

        jobject file; //The file object that return getExternalStorageDirectory
        file = currEnv->CallStaticObjectMethod(envClass,midDataDirectory); //Calls getExternalStorageDirectory
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot execute method getExternalStorageDirectory";
        }

        //Created a Java File Class
        const char * fileClassName;
        fileClassName = "java/io/File"; //Class Name
        jclass fileClass; //File class

        //Gets the class name
        fileClass = currEnv->FindClass(fileClassName); //Finds the class
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot find class java/io/File";
        }
        if (fileClass == NULL)
        {
            return "Error: Java File class is NULL";
        }

        //Call the methhods getAbsolutePath in the Java File class
        jmethodID midGetAbsolutePath; //JNI Method ID
        midGetAbsolutePath = currEnv->GetMethodID(fileClass,"getAbsolutePath","()Ljava/lang/String;"); //Gets the method ID
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot get method ID for getAbsolutePath";
        }

        jstring path; //String path
        path = (jstring)currEnv->CallObjectMethod(file,midGetAbsolutePath); //Calls getAbsolutePath passing the file JObject
        if (currEnv->ExceptionOccurred())
        {
            return "Error: cannot execute method getAbsolutePath";
        }

        //Converts the JString into char*
        const char * ppath;
        ppath = currEnv->GetStringUTFChars(path, 0);

        //Finally create the return QString variable using the char*
        QString ExternalStorageDirectory(ppath);

        //Detach the urrent thread and free memory
        qjniEnv.javaVM()->DetachCurrentThread();

        //Return the path
        return ExternalStorageDirectory;
    }
    else
    {
        return "Error Java VM is empty";
    }
}
