// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame(); // Prompt Player For Guess
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }

}

void UBullCowCartridge::SetupGame()
{
    // Welcoming The Player
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = GetValidWords(Words)[FMath::RandRange(0, GetValidWords(Words).Num() -1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue...")); // Prompt Player For Guess
    PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); // Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    FBullCowCount Score;

    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        Score = GetBullCows(Guess);
        PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
        LoseLife();
        PrintLine(TEXT("The hidden word is %i letters long. \nYou have %i lives remaining"), HiddenWord.Len(), Lives);
        return;
    }
    
    // Check If Isogram
    if (!IsIsogram(Guess))
    {
        Score = GetBullCows(Guess);
        PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

        LoseLife();
        PrintLine(TEXT("No repeating letters, guess again. \nYou have %i lives left"), Lives);
        return;
    }

    // Remove Life
    LoseLife();
            
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hiddenword was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison <Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    char temp[5];
    int counter = 0;

    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            //PrintLine(TEXT("%i"), HiddenWord[GuessIndex]); //Debug prints ascii index of bull
            temp[counter] = HiddenWord[GuessIndex];
            //PrintLine(TEXT("%i"), temp[counter]); //Debug prints copy of bull value
            counter++;
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                for(int32 i = 0; i <= counter; i++)
                {
                    if(temp[i] == HiddenWord[HiddenIndex])
                    {
                        Count.Cows--;
                        break;
                    }
                }
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}

void UBullCowCartridge::LoseLife()
{
    PrintLine(TEXT("Lost a life!"));
    --Lives;
}