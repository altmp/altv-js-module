/// <reference types="@altv/types-shared"/>
/// <reference types="@altv/types-natives"/>
import * as altShared from "alt-shared";
import * as altNatives from "natives";

declare global {
    export const alt: typeof altShared;
    export const native: typeof altNatives;
}
