# Operating Systems

## Install Java

Java comprises of JRE (Java Runtime Environment) and JDK (Java Development
Toolkit)

JRE is for development. JDK is for building/compiling.

```
# install latest jre and jdk in arch-based system
sudo pacman -S jre-openjdk jdk-openjdk

# install javafx (not needed)
sudo pacman -S java-openjfx

# check java version
java -version
```

## Compile and Run Java Program

Compile a java program with the javac compiler that's provided by JDK and run
the program with the java command that's provided by JRE.

```
# compile the HelloWorld program
javac HelloWorld.java

# run the HelloWorld program
java HelloWorld
```

